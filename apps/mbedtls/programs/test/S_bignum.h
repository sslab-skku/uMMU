#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/bignum.h"
#include "mbedtls/bn_mul.h"
#include "mbedtls/platform_util.h"

#include <limits.h>
#include <string.h>

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_printf     printf
#define mbedtls_calloc    calloc
#define mbedtls_free       free
#endif

#define MPI_VALIDATE_RET( cond )                                       \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_MPI_BAD_INPUT_DATA )
#define MPI_VALIDATE( cond )                                           \
    MBEDTLS_INTERNAL_VALIDATE( cond )

#define ciL    (sizeof(mbedtls_mpi_uint))         /* chars in limb  */
#define biL    (ciL << 3)               /* bits  in limb  */
#define biH    (ciL << 2)               /* half limb size */

#define MPI_SIZE_T_MAX  ( (size_t) -1 ) /* SIZE_T_MAX is not standard */

/*
 * Convert between bits/chars and number of limbs
 * Divide first in order to avoid potential overflows
 */
#define BITS_TO_LIMBS(i)  ( (i) / biL + ( (i) % biL != 0 ) )
#define CHARS_TO_LIMBS(i) ( (i) / ciL + ( (i) % ciL != 0 ) )

#define S__mbedtls_calloc anno_secret_calloc

/*
 * Enlarge to the specified number of limbs
 */
static int S__mbedtls_mpi_grow( mbedtls_mpi *X, size_t nblimbs )
{
    mbedtls_mpi_uint *p;
    MPI_VALIDATE_RET( X != NULL );

    if( nblimbs > MBEDTLS_MPI_MAX_LIMBS )
        return( MBEDTLS_ERR_MPI_ALLOC_FAILED );

    if( X->n < nblimbs )
    {
        if( ( p = (mbedtls_mpi_uint*)S__mbedtls_calloc( nblimbs, ciL ) ) == NULL )
            return( MBEDTLS_ERR_MPI_ALLOC_FAILED );

        if( X->p != NULL )
        {
            memcpy( p, X->p, X->n * ciL );
            mbedtls_mpi_zeroize( X->p, X->n );
            mbedtls_free( X->p );
        }

        X->n = nblimbs;
        X->p = p;
    }

    return( 0 );
}

/*
 * Resize down as much as possible,
 * while keeping at least the specified number of limbs
 */
static int S__mbedtls_mpi_shrink( mbedtls_mpi *X, size_t nblimbs )
{
#if 0
    mbedtls_mpi_uint *p;
    size_t i;
    MPI_VALIDATE_RET( X != NULL );

    if( nblimbs > MBEDTLS_MPI_MAX_LIMBS )
        return( MBEDTLS_ERR_MPI_ALLOC_FAILED );

    /* Actually resize up if there are currently fewer than nblimbs limbs. */
    if( X->n <= nblimbs )
        return( mbedtls_mpi_grow( X, nblimbs ) );
    /* After this point, then X->n > nblimbs and in particular X->n > 0. */

    for( i = X->n - 1; i > 0; i-- )
        if( X->p[i] != 0 )
            break;
    i++;

    if( i < nblimbs )
        i = nblimbs;

    if( ( p = (mbedtls_mpi_uint*)mbedtls_calloc( i, ciL ) ) == NULL )
        return( MBEDTLS_ERR_MPI_ALLOC_FAILED );

    if( X->p != NULL )
    {
        memcpy( p, X->p, i * ciL );
        mbedtls_mpi_zeroize( X->p, X->n );
        mbedtls_free( X->p );
    }

    X->n = i;
    X->p = p;
#endif
    return( 0 );
}

/*
 * Copy the contents of Y into X
 */
static int S__mbedtls_mpi_copy( mbedtls_mpi *X, const mbedtls_mpi *Y )
{
    int ret = 0;
    size_t i;
    MPI_VALIDATE_RET( X != NULL );
    MPI_VALIDATE_RET( Y != NULL );

    if( X == Y )
        return( 0 );

    if( Y->n == 0 )
    {
        mbedtls_mpi_free( X );
        return( 0 );
    }

    for( i = Y->n - 1; i > 0; i-- )
        if( Y->p[i] != 0 )
            break;
    i++;

    X->s = Y->s;

    if( X->n < i )
    {
        MBEDTLS_MPI_CHK( S__mbedtls_mpi_grow( X, i ) );
    }
    else
    {
        memset( X->p + i, 0, ( X->n - i ) * ciL );
    }

    memcpy( X->p, Y->p, i * ciL );

cleanup:

    return( ret );
}

/*
 * Swap the contents of X and Y
 */
static void S__mbedtls_mpi_swap( mbedtls_mpi *X, mbedtls_mpi *Y )
{
    mbedtls_mpi T;
    MPI_VALIDATE( X != NULL );
    MPI_VALIDATE( Y != NULL );

    T.s=X->s;
    T.n=X->n;
    T.p=X->p;

    X->s=Y->s;
    X->n=Y->n;
    X->p=Y->p;

    Y->s=T.s;
    Y->n=T.n;
    Y->p=T.p;
}

/*
 * Set value from integer
 */
static int S__mbedtls_mpi_lset( mbedtls_mpi *X, mbedtls_mpi_sint z )
{
    int ret;
    MPI_VALIDATE_RET( X != NULL );

    MBEDTLS_MPI_CHK( S__mbedtls_mpi_grow( X, 1 ) );
    memset( X->p, 0, X->n * ciL );

    X->p[0] = ( z < 0 ) ? -z : z;
    X->s    = ( z < 0 ) ? -1 : 1;

cleanup:

    return( ret );
}

/*
 * Set a bit to a specific value of 0 or 1
 */
static int S__mbedtls_mpi_set_bit( mbedtls_mpi *X, size_t pos, unsigned char val )
{
    int ret = 0;
    size_t off = pos / biL;
    size_t idx = pos % biL;
    MPI_VALIDATE_RET( X != NULL );

    if( val != 0 && val != 1 )
        return( MBEDTLS_ERR_MPI_BAD_INPUT_DATA );

    if( X->n * biL <= pos )
    {
        if( val == 0 )
            return( 0 );

        MBEDTLS_MPI_CHK( S__mbedtls_mpi_grow( X, off + 1 ) );
    }

    X->p[off] &= ~( (mbedtls_mpi_uint) 0x01 << idx );
    X->p[off] |= (mbedtls_mpi_uint) val << idx;

cleanup:

    return( ret );
}


static int S__mbedtls_mpi_read_string( mbedtls_mpi *X, int radix, const char *s )
{
    int ret;
    size_t i, j, slen, n;
    int sign = 1;
    mbedtls_mpi_uint d;
    mbedtls_mpi T;
    MPI_VALIDATE_RET( X != NULL );
    MPI_VALIDATE_RET( s != NULL );

    if( radix < 2 || radix > 16 )
        return( MBEDTLS_ERR_MPI_BAD_INPUT_DATA );

    mbedtls_mpi_init( &T );

    if( s[0] == '-' )
    {
        ++s;
        sign = -1;
    }

    slen = strlen( s );

    if( radix == 16 )
    {
        if( slen > MPI_SIZE_T_MAX >> 2 )
            return( MBEDTLS_ERR_MPI_BAD_INPUT_DATA );

        n = BITS_TO_LIMBS( slen << 2 );

        MBEDTLS_MPI_CHK( S__mbedtls_mpi_grow( X, n ) );
        MBEDTLS_MPI_CHK( S__mbedtls_mpi_lset( X, 0 ) );

        for( i = slen, j = 0; i > 0; i--, j++ )
        {
            MBEDTLS_MPI_CHK( mpi_get_digit( &d, radix, s[i - 1] ) );
            X->p[j / ( 2 * ciL )] |= d << ( ( j % ( 2 * ciL ) ) << 2 );
        }
    }
    else
    {
        MBEDTLS_MPI_CHK( S__mbedtls_mpi_lset( X, 0 ) );

        for( i = 0; i < slen; i++ )
        {
            MBEDTLS_MPI_CHK( mpi_get_digit( &d, radix, s[i] ) );
            MBEDTLS_MPI_CHK( S__mbedtls_mpi_mul_int( &T, X, radix ) );
            MBEDTLS_MPI_CHK( S__mbedtls_mpi_add_int( X, &T, d ) );
        }
    }

    if( sign < 0 && mbedtls_mpi_bitlen( X ) != 0 )
        X->s = -1;

cleanup:

    mbedtls_mpi_free( &T );

    return( ret );
}

/* Convert a big-endian byte array aligned to the size of mbedtls_mpi_uint
 * into the storage form used by mbedtls_mpi. */

static mbedtls_mpi_uint mpi_uint_bigendian_to_host_c( mbedtls_mpi_uint x )
{
    uint8_t i;
    unsigned char *x_ptr;
    mbedtls_mpi_uint tmp = 0;

    for( i = 0, x_ptr = (unsigned char*) &x; i < ciL; i++, x_ptr++ )
    {
        tmp <<= CHAR_BIT;
        tmp |= (mbedtls_mpi_uint) *x_ptr;
    }

    return( tmp );
}

static mbedtls_mpi_uint mpi_uint_bigendian_to_host( mbedtls_mpi_uint x )
{
#if defined(__BYTE_ORDER__)

/* Nothing to do on bigendian systems. */
#if ( __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ )
    return( x );
#endif /* __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ */

#if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )

/* For GCC and Clang, have builtins for byte swapping. */
#if defined(__GNUC__) && defined(__GNUC_PREREQ)
#if __GNUC_PREREQ(4,3)
#define have_bswap
#endif
#endif

#if defined(__clang__) && defined(__has_builtin)
#if __has_builtin(__builtin_bswap32)  &&                 \
    __has_builtin(__builtin_bswap64)
#define have_bswap
#endif
#endif

#if defined(have_bswap)
    /* The compiler is hopefully able to statically evaluate this! */
    switch( sizeof(mbedtls_mpi_uint) )
    {
        case 4:
            return( __builtin_bswap32(x) );
        case 8:
            return( __builtin_bswap64(x) );
    }
#endif
#endif /* __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */
#endif /* __BYTE_ORDER__ */

    /* Fall back to C-based reordering if we don't know the byte order
     * or we couldn't use a compiler-specific builtin. */
    return( mpi_uint_bigendian_to_host_c( x ) );
}

static void mpi_bigendian_to_host( mbedtls_mpi_uint * const p, size_t limbs )
{
    mbedtls_mpi_uint *cur_limb_left;
    mbedtls_mpi_uint *cur_limb_right;
    if( limbs == 0 )
        return;

    /*
     * Traverse limbs and
     * - adapt byte-order in each limb
     * - swap the limbs themselves.
     * For that, simultaneously traverse the limbs from left to right
     * and from right to left, as long as the left index is not bigger
     * than the right index (it's not a problem if limbs is odd and the
     * indices coincide in the last iteration).
     */
    for( cur_limb_left = p, cur_limb_right = p + ( limbs - 1 );
         cur_limb_left <= cur_limb_right;
         cur_limb_left++, cur_limb_right-- )
    {
        mbedtls_mpi_uint tmp;
        /* Note that if cur_limb_left == cur_limb_right,
         * this code effectively swaps the bytes only once. */
        tmp             = mpi_uint_bigendian_to_host( *cur_limb_left  );
        *cur_limb_left  = mpi_uint_bigendian_to_host( *cur_limb_right );
        *cur_limb_right = tmp;
    }
}


static int S__mbedtls_mpi_read_binary( mbedtls_mpi *X, const unsigned char *buf, size_t buflen )
{
    int ret;
    size_t const limbs    = CHARS_TO_LIMBS( buflen );
    size_t const overhead = ( limbs * ciL ) - buflen;
    unsigned char *Xp;

    MPI_VALIDATE_RET( X != NULL );
    MPI_VALIDATE_RET( buflen == 0 || buf != NULL );

    /* Ensure that target MPI has exactly the necessary number of limbs */
    if( X->n != limbs )
    {
        mbedtls_mpi_free( X );
        mbedtls_mpi_init( X );
        MBEDTLS_MPI_CHK( S__mbedtls_mpi_grow( X, limbs ) );
    }
    MBEDTLS_MPI_CHK( S__mbedtls_mpi_lset( X, 0 ) );

    /* Avoid calling `memcpy` with NULL source argument,
     * even if buflen is 0. */
    if( buf != NULL )
    {
        Xp = (unsigned char*) X->p;
        memcpy( Xp + overhead, buf, buflen );

        mpi_bigendian_to_host( X->p, limbs );
    }

cleanup:

    return( ret );
}


/*
 * Sliding-window exponentiation: X = A^E mod N  (HAC 14.85)
 */
int mbedtls_mpi_exp_mod( mbedtls_mpi *X, const mbedtls_mpi *A,
                         const mbedtls_mpi *E, const mbedtls_mpi *N,
                         mbedtls_mpi *prec_RR )
{
    int ret;
    size_t wbits, wsize, one = 1;
    size_t i, j, nblimbs;
    size_t bufsize, nbits;
    mbedtls_mpi_uint ei, mm, state;
    mbedtls_mpi RR, T, W[ 1 << MBEDTLS_MPI_WINDOW_SIZE ], WW, Apos;
    int neg;

    MPI_VALIDATE_RET( X != NULL );
    MPI_VALIDATE_RET( A != NULL );
    MPI_VALIDATE_RET( E != NULL );
    MPI_VALIDATE_RET( N != NULL );

    if( mbedtls_mpi_cmp_int( N, 0 ) <= 0 || ( N->p[0] & 1 ) == 0 )
        return( MBEDTLS_ERR_MPI_BAD_INPUT_DATA );

    if( mbedtls_mpi_cmp_int( E, 0 ) < 0 )
        return( MBEDTLS_ERR_MPI_BAD_INPUT_DATA );

    if( mbedtls_mpi_bitlen( E ) > MBEDTLS_MPI_MAX_BITS ||
        mbedtls_mpi_bitlen( N ) > MBEDTLS_MPI_MAX_BITS )
        return ( MBEDTLS_ERR_MPI_BAD_INPUT_DATA );

    /*
     * Init temps and window size
     */
    mpi_montg_init( &mm, N );
    mbedtls_mpi_init( &RR ); mbedtls_mpi_init( &T );
    mbedtls_mpi_init( &Apos );
    mbedtls_mpi_init( &WW );
    memset( W, 0, sizeof( W ) );

    i = mbedtls_mpi_bitlen( E );

    wsize = ( i > 671 ) ? 6 : ( i > 239 ) ? 5 :
            ( i >  79 ) ? 4 : ( i >  23 ) ? 3 : 1;

#if( MBEDTLS_MPI_WINDOW_SIZE < 6 )
    if( wsize > MBEDTLS_MPI_WINDOW_SIZE )
        wsize = MBEDTLS_MPI_WINDOW_SIZE;
#endif

    j = N->n + 1;
    /* All W[i] and X must have at least N->n limbs for the mpi_montmul()
     * and mpi_montred() calls later. Here we ensure that W[1] and X are
     * large enough, and later we'll grow other W[i] to the same length.
     * They must not be shrunk midway through this function!
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_grow( X, j ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_grow( &W[1],  j ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_grow( &T, j * 2 ) );

    /*
     * Compensate for negative A (and correct at the end)
     */
    neg = ( A->s == -1 );
    if( neg )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &Apos, A ) );
        Apos.s = 1;
        A = &Apos;
    }

    /*
     * If 1st call, pre-compute R^2 mod N
     */
    if( prec_RR == NULL || prec_RR->p == NULL )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &RR, 1 ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_shift_l( &RR, N->n * 2 * biL ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &RR, &RR, N ) );

        if( prec_RR != NULL )
            memcpy( prec_RR, &RR, sizeof( mbedtls_mpi ) );
    }
    else
        memcpy( &RR, prec_RR, sizeof( mbedtls_mpi ) );

    /*
     * W[1] = A * R^2 * R^-1 mod N = A * R mod N
     */
    if( mbedtls_mpi_cmp_mpi( A, N ) >= 0 )
        MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &W[1], A, N ) );
    else
        MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &W[1], A ) );
    /* Re-grow W[1] if necessary. This should be only necessary in one corner
     * case: when A == 0 represented with A.n == 0, mbedtls_mpi_copy shrinks
     * W[1] to 0 limbs. */
    MBEDTLS_MPI_CHK( mbedtls_mpi_grow( &W[1], N->n +1 ) );

    mpi_montmul( &W[1], &RR, N, mm, &T );

    /*
     * X = R^2 * R^-1 mod N = R mod N
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( X, &RR ) );
    mpi_montred( X, N, mm, &T );

    if( wsize > 1 )
    {
        /*
         * W[1 << (wsize - 1)] = W[1] ^ (wsize - 1)
         */
        j =  one << ( wsize - 1 );

        MBEDTLS_MPI_CHK( mbedtls_mpi_grow( &W[j], N->n + 1 ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &W[j], &W[1]    ) );

        for( i = 0; i < wsize - 1; i++ )
            mpi_montmul( &W[j], &W[j], N, mm, &T );

        /*
         * W[i] = W[i - 1] * W[1]
         */
        for( i = j + 1; i < ( one << wsize ); i++ )
        {
            MBEDTLS_MPI_CHK( mbedtls_mpi_grow( &W[i], N->n + 1 ) );
            MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &W[i], &W[i - 1] ) );

            mpi_montmul( &W[i], &W[1], N, mm, &T );
        }
    }

    nblimbs = E->n;
    bufsize = 0;
    nbits   = 0;
    wbits   = 0;
    state   = 0;

    while( 1 )
    {
        if( bufsize == 0 )
        {
            if( nblimbs == 0 )
                break;

            nblimbs--;

            bufsize = sizeof( mbedtls_mpi_uint ) << 3;
        }

        bufsize--;

        ei = (E->p[nblimbs] >> bufsize) & 1;

        /*
         * skip leading 0s
         */
        if( ei == 0 && state == 0 )
            continue;

        if( ei == 0 && state == 1 )
        {
            /*
             * out of window, square X
             */
            mpi_montmul( X, X, N, mm, &T );
            continue;
        }

        /*
         * add ei to current window
         */
        state = 2;

        nbits++;
        wbits |= ( ei << ( wsize - nbits ) );

        if( nbits == wsize )
        {
            /*
             * X = X^wsize R^-1 mod N
             */
            for( i = 0; i < wsize; i++ )
                mpi_montmul( X, X, N, mm, &T );

            /*
             * X = X * W[wbits] R^-1 mod N
             */
            MBEDTLS_MPI_CHK( mpi_select( &WW, W, (size_t) 1 << wsize, wbits ) );
            mpi_montmul( X, &WW, N, mm, &T );

            state--;
            nbits = 0;
            wbits = 0;
        }
    }

    /*
     * process the remaining bits
     */
    for( i = 0; i < nbits; i++ )
    {
        mpi_montmul( X, X, N, mm, &T );

        wbits <<= 1;

        if( ( wbits & ( one << wsize ) ) != 0 )
            mpi_montmul( X, &W[1], N, mm, &T );
    }

    /*
     * X = A^E * R * R^-1 mod N = A^E mod N
     */
    mpi_montred( X, N, mm, &T );

    if( neg && E->n != 0 && ( E->p[0] & 1 ) != 0 )
    {
        X->s = -1;
        MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( X, N, X ) );
    }

cleanup:

    for( i = ( one << ( wsize - 1 ) ); i < ( one << wsize ); i++ )
        mbedtls_mpi_free( &W[i] );

    mbedtls_mpi_free( &W[1] ); mbedtls_mpi_free( &T ); mbedtls_mpi_free( &Apos );
    mbedtls_mpi_free( &WW );

    if( prec_RR == NULL || prec_RR->p == NULL )
        mbedtls_mpi_free( &RR );

    return( ret );
}

/*
 * Greatest common divisor: G = gcd(A, B)  (HAC 14.54)
 */
int mbedtls_mpi_gcd( mbedtls_mpi *G, const mbedtls_mpi *A, const mbedtls_mpi *B )
{
    int ret;
    size_t lz, lzt;
    mbedtls_mpi TG, TA, TB;

    MPI_VALIDATE_RET( G != NULL );
    MPI_VALIDATE_RET( A != NULL );
    MPI_VALIDATE_RET( B != NULL );

    mbedtls_mpi_init( &TG ); mbedtls_mpi_init( &TA ); mbedtls_mpi_init( &TB );

    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &TA, A ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &TB, B ) );

    lz = mbedtls_mpi_lsb( &TA );
    lzt = mbedtls_mpi_lsb( &TB );

    /* The loop below gives the correct result when A==0 but not when B==0.
     * So have a special case for B==0. Leverage the fact that we just
     * calculated the lsb and lsb(B)==0 iff B is odd or 0 to make the test
     * slightly more efficient than cmp_int(). */
    if( lzt == 0 && mbedtls_mpi_get_bit( &TB, 0 ) == 0 )
    {
        ret = mbedtls_mpi_copy( G, A );
        goto cleanup;
    }

    if( lzt < lz )
        lz = lzt;

    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &TA, lz ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &TB, lz ) );

    TA.s = TB.s = 1;

    /* We mostly follow the procedure described in HAC 14.54, but with some
     * minor differences:
     * - Sequences of multiplications or divisions by 2 are grouped into a
     *   single shift operation.
     * - The procedure in HAC assumes that 0 < TB <= TA.
     *     - The condition TB <= TA is not actually necessary for correctness.
     *       TA and TB have symmetric roles except for the loop termination
     *       condition, and the shifts at the beginning of the loop body
     *       remove any significance from the ordering of TA vs TB before
     *       the shifts.
     *     - If TA = 0, the loop goes through 0 iterations and the result is
     *       correctly TB.
     *     - The case TB = 0 was short-circuited above.
     *
     * For the correctness proof below, decompose the original values of
     * A and B as
     *   A = sa * 2^a * A' with A'=0 or A' odd, and sa = +-1
     *   B = sb * 2^b * B' with B'=0 or B' odd, and sb = +-1
     * Then gcd(A, B) = 2^{min(a,b)} * gcd(A',B'),
     * and gcd(A',B') is odd or 0.
     *
     * At the beginning, we have TA = |A|/2^a and TB = |B|/2^b.
     * The code maintains the following invariant:
     *     gcd(A,B) = 2^k * gcd(TA,TB) for some k   (I)
     */

    /* Proof that the loop terminates:
     * At each iteration, either the right-shift by 1 is made on a nonzero
     * value and the nonnegative integer bitlen(TA) + bitlen(TB) decreases
     * by at least 1, or the right-shift by 1 is made on zero and then
     * TA becomes 0 which ends the loop (TB cannot be 0 if it is right-shifted
     * since in that case TB is calculated from TB-TA with the condition TB>TA).
     */
    while( mbedtls_mpi_cmp_int( &TA, 0 ) != 0 )
    {
        /* Divisions by 2 preserve the invariant (I). */
        MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &TA, mbedtls_mpi_lsb( &TA ) ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &TB, mbedtls_mpi_lsb( &TB ) ) );

        /* Set either TA or TB to |TA-TB|/2. Since TA and TB are both odd,
         * TA-TB is even so the division by 2 has an integer result.
         * Invariant (I) is preserved since any odd divisor of both TA and TB
         * also divides |TA-TB|/2, and any odd divisor of both TA and |TA-TB|/2
         * also divides TB, and any odd divisior of both TB and |TA-TB|/2 also
         * divides TA.
         */
        if( mbedtls_mpi_cmp_mpi( &TA, &TB ) >= 0 )
        {
            MBEDTLS_MPI_CHK( mbedtls_mpi_sub_abs( &TA, &TA, &TB ) );
            MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &TA, 1 ) );
        }
        else
        {
            MBEDTLS_MPI_CHK( mbedtls_mpi_sub_abs( &TB, &TB, &TA ) );
            MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &TB, 1 ) );
        }
        /* Note that one of TA or TB is still odd. */
    }

    /* By invariant (I), gcd(A,B) = 2^k * gcd(TA,TB) for some k.
     * At the loop exit, TA = 0, so gcd(TA,TB) = TB.
     * - If there was at least one loop iteration, then one of TA or TB is odd,
     *   and TA = 0, so TB is odd and gcd(TA,TB) = gcd(A',B'). In this case,
     *   lz = min(a,b) so gcd(A,B) = 2^lz * TB.
     * - If there was no loop iteration, then A was 0, and gcd(A,B) = B.
     *   In this case, lz = 0 and B = TB so gcd(A,B) = B = 2^lz * TB as well.
     */

    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_l( &TB, lz ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( G, &TB ) );

cleanup:

    mbedtls_mpi_free( &TG ); mbedtls_mpi_free( &TA ); mbedtls_mpi_free( &TB );

    return( ret );
}

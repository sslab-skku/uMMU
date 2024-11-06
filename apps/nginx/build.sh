#!/bin/bash -eux

cd "$(dirname "$0")"

# 1. Build
export CC=wllvm
export LLVM_COMPILER=clang
export LLVM_CC_NAME=musl-clang
CMI="$(realpath ../../cmi/)"
LIBMD="$(realpath ../libmd/)"


DO_CONFIGURE=${1:-1}

if [ $DO_CONFIGURE -eq 1 ];then
    CONFIG=(
        --with-cc-opt="-O0 -I$CMI/include -fno-stack-protector -mno-avx -mno-sse -Wno-sign-compare -Wno-unused-function -Wno-unused-command-line-argument"
        # --without-openssl # do not use libcrypto.so

        # --without-pcre2
        --without-pcre
        --without-select_module
        --without-poll_module
        # --with-file-aio
        --without-http_gzip_module

        --without-http_rewrite_module

        --without-http_access_module
  	    --without-http_auth_basic_module
  	    --without-http_autoindex_module
  	    --without-http_browser_module
  	    --without-http_charset_module
  	    --without-http_empty_gif_module
  	    --without-http_geo_module
  	    --without-http_memcached_module
  	    --without-http_map_module
  	    --without-http_ssi_module
  	    --without-http_split_clients_module
  	    --without-http_fastcgi_module
  	    --without-http_uwsgi_module
  	    --without-http_userid_module
  	    --without-http_scgi_module
  	    --without-mail_pop3_module
  	    --without-mail_imap_module
  	    --without-mail_smtp_module
    )
    ./auto/configure "${CONFIG[@]}"
fi

[ ${DO_CLEAN:-0} -eq 1 ] && make clean
make -B -j$(nproc)


# 2. Extract bitcode
outdir="$PWD/out"
mkdir -p ${outdir}

target_list=(
    objs/nginx
)

for f in "${target_list[@]}";do
    outf=$outdir/$(basename $f)
    cp $f $outdir
    extract-bc $outf
    llvm-dis $outf.bc
    llvm-nm -u $outf.bc > $outf.llvm-nm.txt
done


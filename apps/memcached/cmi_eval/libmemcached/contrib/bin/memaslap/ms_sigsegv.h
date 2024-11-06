/*
    +--------------------------------------------------------------------+
    | libmemcached-awesome - C/C++ Client Library for memcached          |
    +--------------------------------------------------------------------+
    | Redistribution and use in source and binary forms, with or without |
    | modification, are permitted under the terms of the BSD license.    |
    | You should have received a copy of the license in a bundled file   |
    | named LICENSE; in case you did not receive a copy you can review   |
    | the terms online at: https://opensource.org/licenses/BSD-3-Clause  |
    +--------------------------------------------------------------------+
    | Copyright (c) 2006-2014 Brian Aker   https://datadifferential.com/ |
    | Copyright (c) 2020 Michael Wallner   <mike@php.net>                |
    +--------------------------------------------------------------------+
*/

#ifndef MS_SIGSEGV_H
#define MS_SIGSEGV_H

#ifdef __cplusplus
extern "C" {
#endif

/* redirect signal seg */
int ms_setup_sigsegv(void);

/* redirect signal pipe */
int ms_setup_sigpipe(void);

/* redirect signal int */
int ms_setup_sigint(void);

#ifdef __cplusplus
}
#endif

#endif /* end of MS_SIGSEGV_H */

/*
 * Copyright (c) 2015 - 2019 Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY LOG OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <memkind.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define PMEM_MAX_SIZE (1024 * 1024 * 32)

static char path[PATH_MAX]="/tmp/";

static void print_err_message(int err)
{
    char error_message[MEMKIND_ERROR_MESSAGE_SIZE];
    memkind_error_message(err, error_message, MEMKIND_ERROR_MESSAGE_SIZE);
    fprintf(stderr, "%s\n", error_message);
}

int main(int argc, char *argv[])
{
    struct memkind *pmem_kind = NULL;
    int err = 0;

    if (argc > 2) {
        fprintf(stderr, "Usage: %s [pmem_kind_dir_path]\n", argv[0]);
        return 1;
    } else if (argc == 2 && (realpath(argv[1], path) == NULL)) {
        fprintf(stderr, "Incorrect pmem_kind_dir_path %s\n", argv[1]);
        return 1;
    }

    fprintf(stdout,
            "This example shows how to allocate memory and possibility to exceed pmem kind size."
            "\nPMEM kind directory: %s\n", path);

    // Create PMEM partition with specific size
    err = memkind_create_pmem(path, PMEM_MAX_SIZE, &pmem_kind);
    if (err) {
        print_err_message(err);
        return 1;
    }
    fprintf(stdout, "offset: %ld.\n", memkind_get_offset(pmem_kind));

    char *pmem_str1 = NULL;
    char *pmem_str2 = NULL;
    char *pmem_str3 = NULL;

    // Allocate 512 Bytes of 32 MB available
    pmem_str1 = (char *)memkind_malloc(pmem_kind, 100);
    if (pmem_str1 == NULL) {
        fprintf(stderr, "Unable to allocate pmem string (pmem_str1).\n");
        return 1;
    }
    fprintf(stdout, "arena_addr: %p, offset: %ld.\n", memkind_get_arena_addr(pmem_kind), memkind_get_offset(pmem_kind));
    fprintf(stdout, "apply 512: pmem_str1: %p.\n", pmem_str1);
    fprintf(stdout, "---------------------------------------------------\n");

    pmem_str2 = (char *)memkind_malloc(pmem_kind, 60);
    if (pmem_str2 == NULL) {
        fprintf(stderr, "Unable to allocate pmem string (pmem_str1).\n");
        return 1;
    }
    fprintf(stdout, "arena_addr: %p, offset: %ld.\n", memkind_get_arena_addr(pmem_kind), memkind_get_offset(pmem_kind));
    fprintf(stdout, "apply 512: pmem_str1: %p.\n", pmem_str2);
    fprintf(stdout, "---------------------------------------------------\n");

    pmem_str3 = (char *)memkind_malloc(pmem_kind, 512);
    if (pmem_str3 == NULL) {
        fprintf(stderr, "Unable to allocate pmem string (pmem_str1).\n");
        return 1;
    }
    fprintf(stdout, "arena_addr: %p, offset: %ld.\n", memkind_get_arena_addr(pmem_kind), memkind_get_offset(pmem_kind));
    fprintf(stdout, "apply 512: pmem_str3: %p.\n", pmem_str3);
    fprintf(stdout, "---------------------------------------------------\n");

    memkind_free(pmem_kind, pmem_str1);
    fprintf(stdout, "free %p.\n", pmem_str1);
    fprintf(stdout, "arena_addr: %p, offset: %ld.\n", memkind_get_arena_addr(pmem_kind), memkind_get_offset(pmem_kind));
    fprintf(stdout, "apply 512: pmem_str1: %p.\n", pmem_str1);
    fprintf(stdout, "---------------------------------------------------\n");


    err = memkind_destroy_kind(pmem_kind);
    if (err) {
        print_err_message(err);
        return 1;
    }

    fprintf(stdout, "Memory was successfully allocated and released.\n");

    return 0;
}

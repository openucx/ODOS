/*
 * Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
 *
 * This software product is a proprietary product of NVIDIA CORPORATION &
 * AFFILIATES (the "Company") and all right, title, and interest in and to the
 * software product, including all associated intellectual property rights, are
 * and shall remain exclusively with the Company.
 *
 * This software product is governed by the End User License Agreement
 * provided with the software product.
 *
 */

#ifndef DOCA_COMMON_DEFINES_H_
#define DOCA_COMMON_DEFINES_H_

#ifdef __GNUC__
#define doca_likely(x) __builtin_expect(!!(x), 1)
#define doca_unlikely(x) __builtin_expect(!!(x), 0)
#else
#define doca_likely(x) (x)
#define doca_unlikely(x) (x)
#endif

#endif /* DOCA_COMMON_DEFINES_H_ */

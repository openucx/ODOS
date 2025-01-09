/*
 * Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES, ALL RIGHTS RESERVED.
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

/**
 * @file doca_config.h
 * @page config
 * @defgroup CONFIG Environment Configurations
 *
 * @{
 */

#ifndef DOCA_CONFIG_H_
#define DOCA_CONFIG_H_

/* Auto-generated configuration file */
#include <doca_build_config.h>

/** @brief declares the support/need for compatibility helper utils */
#define DOCA_COMPAT_HELPERS

/* DPA Device uses static compilation and doesn't need the compat helper utils */
#ifdef DOCA_DPA_DEVICE
#undef DOCA_COMPAT_HELPERS
#endif /* DOCA_DPA_DEVICE */

/** @} */

#endif /* DOCA_CONFIG_H_ */

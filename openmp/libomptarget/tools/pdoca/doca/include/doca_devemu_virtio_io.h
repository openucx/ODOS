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
 * @file doca_devemu_virtio_io.h
 * @page doca devemu virtio_io
 * @defgroup VIRTIO Virtio core library
 *
 * DOCA VIRTIO IO context
 *
 * @{
 */

#ifndef DOCA_DEVEMU_VIRTIO_IO_H_
#define DOCA_DEVEMU_VIRTIO_IO_H_

#include <stdint.h>

#include <doca_error.h>
#include <doca_dev.h>
#include <doca_devemu_pci.h>
#include <doca_devemu_virtio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************************
 * DOCA devemu Virtio IO context API
 *********************************************************************************************************************/

/**
 * @brief Convert DOCA Virtio device IO context instance into doca context.
 *
 * @param [in] io
 * DOCA Virtio device IO context instance. This must remain valid until after the context is no longer required.
 *
 * @return
 * doca ctx upon success, NULL otherwise.
 */
DOCA_EXPERIMENTAL
struct doca_ctx *doca_devemu_virtio_io_as_ctx(struct doca_devemu_virtio_io *io);


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* DOCA_DEVEMU_VIRTIO_IO_H_ */

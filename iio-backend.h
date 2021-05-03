/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * libiio - Library for interfacing industrial I/O (IIO) devices
 *
 * Copyright (C) 2020 Analog Devices, Inc.
 */

#ifndef __IIO_BACKEND_H__
#define __IIO_BACKEND_H__

#include <iio.h>

#include <stdbool.h>

#define __api __iio_api

struct iio_device;
struct iio_context;
struct iio_channel;
struct iio_context_pdata;
struct iio_device_pdata;
struct iio_channel_pdata;

enum iio_backend_api_ver {
	IIO_BACKEND_API_V1 = 1,
};

enum iio_attr_type {
	IIO_ATTR_TYPE_DEVICE = 0,
	IIO_ATTR_TYPE_DEBUG,
	IIO_ATTR_TYPE_BUFFER,
};

struct iio_backend_ops {
	struct iio_context * (*clone)(const struct iio_context *ctx);
	ssize_t (*read)(const struct iio_device *dev, void *dst, size_t len,
			uint32_t *mask, size_t words);
	ssize_t (*write)(const struct iio_device *dev,
			const void *src, size_t len);
	int (*open)(const struct iio_device *dev,
			size_t samples_count, bool cyclic);
	int (*close)(const struct iio_device *dev);
	int (*get_fd)(const struct iio_device *dev);
	int (*set_blocking_mode)(const struct iio_device *dev, bool blocking);

	void (*cancel)(const struct iio_device *dev);

	int (*set_kernel_buffers_count)(const struct iio_device *dev,
			unsigned int nb_blocks);
	ssize_t (*get_buffer)(const struct iio_device *dev,
			void **addr_ptr, size_t bytes_used,
			uint32_t *mask, size_t words);

	ssize_t (*read_device_attr)(const struct iio_device *dev,
			const char *attr, char *dst, size_t len, enum iio_attr_type);
	ssize_t (*write_device_attr)(const struct iio_device *dev,
			const char *attr, const char *src,
			size_t len, enum iio_attr_type);
	ssize_t (*read_channel_attr)(const struct iio_channel *chn,
			const char *attr, char *dst, size_t len);
	ssize_t (*write_channel_attr)(const struct iio_channel *chn,
			const char *attr, const char *src, size_t len);

	int (*get_trigger)(const struct iio_device *dev,
			const struct iio_device **trigger);
	int (*set_trigger)(const struct iio_device *dev,
			const struct iio_device *trigger);

	void (*shutdown)(struct iio_context *ctx);

	char * (*get_description)(const struct iio_context *ctx);

	int (*get_version)(const struct iio_context *ctx, unsigned int *major,
			unsigned int *minor, char git_tag[8]);

	int (*set_timeout)(struct iio_context *ctx, unsigned int timeout);
};

/**
 * struct iio_backend - IIO backend object (API version 1)
 * @api_version			API version for interfacing with libiio core library
 * @name			Name of this backend
 * @uri_prefix			URI prefix for this backend
 * @ops				Reference to backend ops
 */
struct iio_backend {
	unsigned int			api_version;
	const char			*name;
	const char			*uri_prefix;
	const struct iio_backend_ops	*ops;
};

struct iio_context * iio_context_create_from_backend(
		const struct iio_backend *backend,
		const char *description);

__api struct iio_context_pdata *
iio_context_get_pdata(const struct iio_context *ctx);

__api void
iio_context_set_pdata(struct iio_context *ctx, struct iio_context_pdata *data);

__api struct iio_device_pdata *
iio_device_get_pdata(const struct iio_device *dev);

__api void
iio_device_set_pdata(struct iio_device *dev, struct iio_device_pdata *data);

__api struct iio_channel_pdata *
iio_channel_get_pdata(const struct iio_channel *chn);

__api void
iio_channel_set_pdata(struct iio_channel *chn, struct iio_channel_pdata *data);

#undef __api

#endif /* __IIO_BACKEND_H__ */

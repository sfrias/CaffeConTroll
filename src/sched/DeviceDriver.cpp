
#include "DeviceDriver.h"

#include "../kernels/include.hxx"

/**
 * This is necessary for template to be instantiated.
 */
template void DeviceDriver::pmap2d_read_coalesce<_fpmap_id,_fmap_lower>(DeviceMemoryPointer * dst, 
	DeviceMemoryPointer * src, const struct PMapHelper args);

template void DeviceDriver::pmap2d_read_coalesce<_fpmap_id,_fmap_remap>(DeviceMemoryPointer * dst, 
	DeviceMemoryPointer * src, const struct PMapHelper args);


template void DeviceDriver::parallel_map<_f_idx_strid4_copy,_f_strid4_copy>
	(DeviceMemoryPointer * dst, DeviceMemoryPointer * src, size_t src_skip, 
		DeviceMemoryPointer * const f_dst_pos_curry, DeviceMemoryPointer * const func_curry);

template void DeviceDriver::sapply<_f_add_one>(DeviceMemoryPointer * dst, DeviceMemoryPointer * const func_curry);

template void DeviceDriver::sapply<_f_set>(DeviceMemoryPointer * dst, DeviceMemoryPointer * const func_curry);

template void DeviceDriver::selementwise_reduce2<_f_reduce>(DeviceMemoryPointer * dst, 
	DeviceMemoryPointer * src1, DeviceMemoryPointer * src2, DeviceMemoryPointer * const func_curry);

template void DeviceDriver::selementwise_reduce2<_f_reduce_mul>(DeviceMemoryPointer * dst, 
	DeviceMemoryPointer * src1, DeviceMemoryPointer * src2, DeviceMemoryPointer * const func_curry);

template void DeviceDriver::selementwise_reduce2<_f_reduce_tanhgrad>(DeviceMemoryPointer * dst, 
	DeviceMemoryPointer * src1, DeviceMemoryPointer * src2, DeviceMemoryPointer * const func_curry);

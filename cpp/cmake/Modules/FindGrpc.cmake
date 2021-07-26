# - Try to find Grpc
# Once done, this will define
#
#  Grpc_FOUND - system has Grpc
#  Grpc_INCLUDE_DIRS - the Grpc include directories
#  Grpc_LIBRARIES - link these to use Grpc

SET(Grpc_FOUND FALSE)


if(NOT Grpc_ROOT)
	set(Grpc_ROOT /opt/grpc_v1.38.1)
endif()


if(NOT (EXISTS ${Grpc_ROOT}))
	return()
endif()

find_file (DEBIAN_FOUND debian_version debconf.conf PATHS /etc)

set (Grpc_INCLUDE_DIRS
	${Grpc_ROOT}/include
	${Grpc_ROOT}/third_party/protobuf/src
	)

if (DEBIAN_FOUND)
	set(Absl_LIBRARIES_ROOT ${Grpc_ROOT}/lib)
else (DEBIAN_FOUND)
	set(Absl_LIBRARIES_ROOT ${Grpc_ROOT}/lib64)
endif (DEBIAN_FOUND)

set (Grpc_LIBRARIES
	${Grpc_ROOT}/build/libgrpc++.a
	${Grpc_ROOT}/build/libgrpc.a
	${Grpc_ROOT}/build/libgrpc_unsecure.a
	${Grpc_ROOT}/build/libgrpc++_reflection.a
	${Grpc_ROOT}/build/libgpr.a
	${Grpc_ROOT}/build/libaddress_sorting.a
	${Grpc_ROOT}/build/libupb.a
	${Grpc_ROOT}/build/third_party/re2/libre2.a
	${Grpc_ROOT}/build/third_party/zlib/libz.a
	${Grpc_ROOT}/build/third_party/protobuf/libprotobuf.a
	${Grpc_ROOT}/build/third_party/cares/cares/lib/libcares.a
	${Absl_LIBRARIES_ROOT}/libabsl_bad_any_cast_impl.a
	${Absl_LIBRARIES_ROOT}/libabsl_bad_optional_access.a
	${Absl_LIBRARIES_ROOT}/libabsl_bad_variant_access.a
	${Absl_LIBRARIES_ROOT}/libabsl_city.a
	${Absl_LIBRARIES_ROOT}/libabsl_civil_time.a
	${Absl_LIBRARIES_ROOT}/libabsl_cord.a
	${Absl_LIBRARIES_ROOT}/libabsl_examine_stack.a
	${Absl_LIBRARIES_ROOT}/libabsl_exponential_biased.a
	${Absl_LIBRARIES_ROOT}/libabsl_failure_signal_handler.a
	${Absl_LIBRARIES_ROOT}/libabsl_flags.a
	${Absl_LIBRARIES_ROOT}/libabsl_flags_commandlineflag.a
	${Absl_LIBRARIES_ROOT}/libabsl_flags_commandlineflag_internal.a
	${Absl_LIBRARIES_ROOT}/libabsl_flags_config.a
	${Absl_LIBRARIES_ROOT}/libabsl_flags_internal.a
	${Absl_LIBRARIES_ROOT}/libabsl_flags_marshalling.a
	${Absl_LIBRARIES_ROOT}/libabsl_flags_parse.a
	${Absl_LIBRARIES_ROOT}/libabsl_flags_private_handle_accessor.a
	${Absl_LIBRARIES_ROOT}/libabsl_flags_program_name.a
	${Absl_LIBRARIES_ROOT}/libabsl_flags_reflection.a
	${Absl_LIBRARIES_ROOT}/libabsl_flags_usage.a
	${Absl_LIBRARIES_ROOT}/libabsl_flags_usage_internal.a
	${Absl_LIBRARIES_ROOT}/libabsl_hash.a
	${Absl_LIBRARIES_ROOT}/libabsl_hashtablez_sampler.a
	${Absl_LIBRARIES_ROOT}/libabsl_leak_check.a
	${Absl_LIBRARIES_ROOT}/libabsl_leak_check_disable.a
	${Absl_LIBRARIES_ROOT}/libabsl_log_severity.a
	${Absl_LIBRARIES_ROOT}/libabsl_periodic_sampler.a
	${Absl_LIBRARIES_ROOT}/libabsl_random_distributions.a
	${Absl_LIBRARIES_ROOT}/libabsl_random_internal_distribution_test_util.a
	${Absl_LIBRARIES_ROOT}/libabsl_random_internal_platform.a
	${Absl_LIBRARIES_ROOT}/libabsl_random_internal_pool_urbg.a
	${Absl_LIBRARIES_ROOT}/libabsl_random_internal_randen.a
	${Absl_LIBRARIES_ROOT}/libabsl_random_internal_randen_hwaes.a
	${Absl_LIBRARIES_ROOT}/libabsl_random_internal_randen_hwaes_impl.a
	${Absl_LIBRARIES_ROOT}/libabsl_random_internal_randen_slow.a
	${Absl_LIBRARIES_ROOT}/libabsl_random_internal_seed_material.a
	${Absl_LIBRARIES_ROOT}/libabsl_random_seed_gen_exception.a
	${Absl_LIBRARIES_ROOT}/libabsl_random_seed_sequences.a
	${Absl_LIBRARIES_ROOT}/libabsl_raw_hash_set.a
	${Absl_LIBRARIES_ROOT}/libabsl_raw_logging_internal.a
	${Absl_LIBRARIES_ROOT}/libabsl_scoped_set_env.a
	${Absl_LIBRARIES_ROOT}/libabsl_spinlock_wait.a
	${Absl_LIBRARIES_ROOT}/libabsl_status.a
	${Absl_LIBRARIES_ROOT}/libabsl_statusor.a
	${Absl_LIBRARIES_ROOT}/libabsl_strerror.a
	${Absl_LIBRARIES_ROOT}/libabsl_str_format_internal.a
	${Absl_LIBRARIES_ROOT}/libabsl_strings.a
	${Absl_LIBRARIES_ROOT}/libabsl_strings_internal.a
	${Absl_LIBRARIES_ROOT}/libabsl_synchronization.a
	${Absl_LIBRARIES_ROOT}/libabsl_graphcycles_internal.a
	${Absl_LIBRARIES_ROOT}/libabsl_symbolize.a
	${Absl_LIBRARIES_ROOT}/libabsl_stacktrace.a
	${Absl_LIBRARIES_ROOT}/libabsl_debugging_internal.a
	${Absl_LIBRARIES_ROOT}/libabsl_demangle_internal.a
	${Absl_LIBRARIES_ROOT}/libabsl_throw_delegate.a
	${Absl_LIBRARIES_ROOT}/libabsl_time.a
	${Absl_LIBRARIES_ROOT}/libabsl_time_zone.a
	${Absl_LIBRARIES_ROOT}/libabsl_wyhash.a
	${Absl_LIBRARIES_ROOT}/libabsl_malloc_internal.a
	${Absl_LIBRARIES_ROOT}/libabsl_base.a
	${Absl_LIBRARIES_ROOT}/libabsl_int128.a
	)

message("Found Grpc (include ${Grpc_INCLUDE_DIRS}; library: ${Grpc_LIBRARIES})")
set(Grpc_FOUND TRUE)


add_library(Core STATIC
        internal/errors/errors.c
    include/math/matrices/mat3.c
    include/math/units/constants.c
    include/math/units/conversions.c
    include/math/vectors/vec2.c

        internal/ECS/entity.c
        internal/layout/layout.c
    internal/ECS/ECS.c
        internal/ECS/entityContext.c
    internal/registry/registry.c
    internal/utilities/IDMap.c
    internal/utilities/poolMapPair.c
    internal/utilities/pools.c
        engine/resources/resources_eng.h
        internal/resources/images_int.h
        internal/resources/images.c
        internal/resources/types_int.h
        engine/resources/types_eng.h
        internal/resources/resources_int.h
        internal/resources/resources.c
        engine/errors/types_eng.h
        engine/errors/errors_eng.h
)

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/engine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

set_target_properties(Core PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

add_library(CoreInit INTERFACE)
target_include_directories(CoreInit INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/init)

target_link_libraries(Core PRIVATE stb_image)
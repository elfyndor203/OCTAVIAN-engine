
add_library(Core STATIC
    engine/BUILD_MANIFEST.c
    
    include/errors/errors/errorHandling.c
    include/math/matrices/mat3.c
    include/math/units/constants.c
    include/math/units/conversions.c
    include/math/vectors/vec2.c

    internal/ECS/entity/entity.c
    internal/core/core.c
    internal/ECS/ECS.c
    internal/ECS/entity/entityContext.c
    internal/registry/registry.c
    internal/utilities/IDMap.c
    internal/utilities/poolMapPair.c
    internal/utilities/pools.c
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
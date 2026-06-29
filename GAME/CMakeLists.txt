

add_executable(GAME
    internal/gameMain.c
)

target_link_libraries(GAME
        PRIVATE
            OCTAVIANEngine)
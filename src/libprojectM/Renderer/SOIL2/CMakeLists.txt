add_library(SOIL2 OBJECT
        etc1_utils.c
        etc1_utils.h
        image_DXT.c
        image_DXT.h
        image_helper.c
        image_helper.h
        pkm_helper.h
        pvr_helper.h
        SOIL2.c
        SOIL2.h
        stb_image.h
        stb_image_write.h
        stbi_DDS.h
        stbi_DDS_c.h
        stbi_ext.h
        stbi_ext_c.h
        stbi_pkm.h
        stbi_pkm_c.h
        stbi_pvr.h
        stbi_pvr_c.h
        )

target_include_directories(SOIL2
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        )

target_link_libraries(SOIL2
        PUBLIC
        ${PROJECTM_OPENGL_LIBRARIES}
        )
        
set_target_properties(SOIL2 PROPERTIES
        FOLDER libprojectM
        )

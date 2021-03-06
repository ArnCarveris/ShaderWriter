file( GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Test*.cpp )

foreach ( TEST_FILE ${TEST_FILES} )
	get_filename_component( TEST_NAME ${TEST_FILE} NAME_WE )
	add_executable( ${TEST_NAME}
		$<TARGET_OBJECTS:TestCommon>
		${TEST_FILE}
	)
	target_link_libraries( ${TEST_NAME}
		PRIVATE
			sdw::ShaderAST
			sdw::test::Common
			${BinLibraries}
	)

	if ( MSVC )
		target_compile_options( ${TEST_NAME} PRIVATE
			-bigobj
		)
	endif ()

	set_property( TARGET ${TEST_NAME} PROPERTY CXX_STANDARD 17 )
	set_property( TARGET ${TEST_NAME} PROPERTY FOLDER "Tests/ShaderAST" )

	add_test(
		NAME ${TEST_NAME}
		COMMAND ${TEST_NAME}
	)
endforeach ()

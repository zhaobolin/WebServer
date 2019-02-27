set(DIR_LIB_SRCS
		thread.h
		thread.cc
		noncopyable.h
		mutex.h
		condition.h
		countdownlatch.h
		currentthread.h
)

#添加静态库的文件																
add_library(libserver_base ${DIR_LIB_SRCS})

#添加对pthread的依赖
target_link_libraries(libserver_base pthread rt)

set_target_properties(libserver_base PROPERTIES OUTPUT_NAME "server_base")

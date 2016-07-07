
serial_inc = include_directories('include')

serial_lib = library('serial',
	sources: [
		'src/serial.cc',
		'src/impl/unix.cc',
		'src/impl/list_ports/list_ports_linux.cc'
	],
	include_directories: serial_inc
)

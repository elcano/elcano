serial_inc = include_directories('include')

serial_lib = library('serial',
	sources: [
		'src/serial.cc',
		'src/impl/unix.cc',
		'src/impl/win.cc',
		'src/impl/list_ports/list_ports_linux.cc',
		'src/impl/list_ports/list_ports_osx.cc',
		'src/impl/list_ports/list_ports_win.cc'
	],
	include_directories: serial_inc
)

serial = declare_dependency(
	link_with: serial_lib,
	include_directories: serial_inc,
	version: '1.2.1'
)

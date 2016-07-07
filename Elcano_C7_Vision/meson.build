project('elcano-vision', 'cpp')

add_global_arguments('-std=gnu++14', language: 'cpp')
add_global_link_arguments('-lraspicam', '-lraspicam_cv', language: 'cpp')

subdir('serial-1.2.1')

cv = dependency('opencv')

executable('elcano-pi',
	sources: ['main.cc', 'detect-and-draw.cc', 'arduino-usb.cc'],
	dependencies: cv,
	link_with: serial_lib,
	include_directories: serial_inc
)

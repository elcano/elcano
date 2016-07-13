project('elcano-vision', 'cpp',
	license : 'MIT'
)

add_global_arguments('-std=c++14', language: 'cpp')
add_global_link_arguments('-lraspicam', '-lraspicam_cv', language: 'cpp')

subdir('serial-1.2.1')

cv = dependency('opencv')

executable('elcano-pi',
	sources: [
		'main.cc',
		'detect.cc',
		'arduino.cc',
		'transform.cc'
	],
	dependencies: cv,
	link_with: serial_lib,
	include_directories: serial_inc
)

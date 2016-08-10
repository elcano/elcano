project('elcano-vision', 'cpp',
	license: 'MIT',
	default_options: ['cpp_std=c++14']
)

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

transform_test = executable('transform-test',
	sources: [
		'transform.cc',
		'test-transform.cc'
	],
	dependencies: cv
)

test('transform test', tranform_test)

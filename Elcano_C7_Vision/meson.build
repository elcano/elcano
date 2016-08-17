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
	dependencies: [cv, serial]
)

transform_test = executable('transform-test',
	sources: [
		'test-transform.cc',
		'transform.cc'
	],
	dependencies: cv
)

arduino_test = executable('arduino-test',
	sources: [
		'test-arduino.cc',
		'arduino.cc'
	],
	dependencies: serial
)

detect_test = executable('detect-test',
	sources: [
		'test-detect.cc',
		'detect.cc'
	],
	dependencies: cv
)

test('transform test', transform_test)
test('arduino test', arduino_test)
test('detect test', detect_test)

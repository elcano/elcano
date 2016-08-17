project('elcano-vision', 'cpp',
	license: 'MIT',
	default_options: ['cpp_std=c++14']
)

subdir('serial-1.2.1')
cv = dependency('opencv')
rcam = declare_dependency(
	link_args: ['-lraspicam', '-lraspicam_cv'],
	dependencies: cv
)

executable('elcano-pi',
	sources: [
		'main.cc',
		'detect.cc',
		'arduino.cc',
		'transform.cc'
	],
	dependencies: [cv, serial, rcam],
	cpp_pch: 'pch/elcano-pi-pch.hh'
)

transform_test = executable('transform-test',
	sources: [
		'test-transform.cc',
		'transform.cc'
	],
	dependencies: cv,
	cpp_pch: 'pch/transform-test-pch.hh'
)

arduino_test = executable('arduino-test',
	sources: [
		'test-arduino.cc',
		'arduino.cc'
	],
	dependencies: serial,
	cpp_pch: 'pch/arduino-test-pch.hh'
)

detect_test = executable('detect-test',
	sources: [
		'test-detect.cc',
		'detect.cc'
	],
	dependencies: [cv, rcam],
	cpp_pch: 'pch/detect-test-pch.hh'
)

test('transform test', transform_test)
test('arduino test', arduino_test)
test('detect test', detect_test)

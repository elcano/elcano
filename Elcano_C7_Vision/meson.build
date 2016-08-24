project('elcano-vision', 'cpp',
	license: 'MIT',
	default_options: ['cpp_std=c++14'])

subdir('serial-1.2.1')
cv = dependency('opencv')
rcam = declare_dependency(
	link_args: ['-lraspicam', '-lraspicam_cv'],
	dependencies: cv)

flex = generator(find_program('flex'),
	output: '@BASENAME@.yy.cc',
	arguments: ['-o', '@OUTPUT@', '@INPUT@'])

bison = generator(find_program('bison'),
	output: ['@BASENAME@.tab.cc', '@BASENAME@.tab.hh'],
	arguments: ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@'])

arduino_l = flex.process('arduino.l')
arduino_y = bison.process('arduino.y')

executable('elcano-pi',
	sources: [
		'main.cc',
		'detect.cc',
		'arduino.cc',
		'transform.cc',
		arduino_l,
		arduino_y
	],
	dependencies: [cv, serial, rcam],
	cpp_pch: 'pch/elcano-pi-pch.hh')

transform_test = executable('transform-test',
	sources: [
		'test-transform.cc',
		'transform.cc'
	],
	dependencies: cv,
	cpp_pch: 'pch/transform-test-pch.hh')

arduino_test = executable('arduino-test',
	sources: [
		'test-arduino.cc',
		'arduino.cc',
		arduino_l,
		arduino_y
	],
	dependencies: serial,
	cpp_pch: 'pch/arduino-test-pch.hh')

detect_test = executable('detect-test',
	sources: [
		'test-detect.cc',
		'detect.cc'
	],
	dependencies: [cv, rcam],
	cpp_pch: 'pch/detect-test-pch.hh')

test('transform test', transform_test)
test('arduino test', arduino_test)
test('detect test', detect_test)

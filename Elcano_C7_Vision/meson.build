project('elcano-vision', 'cpp')

add_global_arguments('-std=gnu++14', language: 'cpp')
add_global_link_arguments('-lraspicam', '-lraspicam_cv', language: 'cpp')

cv = dependency('opencv')

executable('elcano-pi', 'main.cc', 'detect-and-draw.cc', dependencies: cv)

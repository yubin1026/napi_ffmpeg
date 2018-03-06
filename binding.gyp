{
	'targets': [
	{
		'target_name': 'napi_ffmpeg',
		'sources': [
			'src/napi_ffmpeg.c',
		],
		'conditions': [[
			'OS=="mac"', {
				'libraries': [
					'/usr/local/lib/libavcodec.dylib',
					'/usr/local/lib/libavformat.dylib',
					'/usr/local/lib/libavutil.dylib',
					'/usr/local/lib/libpostproc.dylib',
					'/usr/local/lib/libavfilter.dylib',
					'/usr/local/lib/libavdevice.dylib',
					'/usr/local/lib/libavresample.dylib',
					'/usr/local/lib/libswresample.dylib',
					'/usr/local/lib/libswscale.dylib',
					'/usr/local/lib/libass.dylib'
				],
				'xcode_settings': {
            		'OTHER_LDFLAGS': [
              			'-undefined dynamic_lookup',
              			'-framework Cocoa',
              			'-framework OpenGL',              			
              			'-framework OpenAL',
            		]
          		},
				'include_dirs': [
					'/usr/local/include'
				]
			}
		]],
		'defines': [
			'_THREAD_SAFE',
			'ENABLE_ARG_CHECKING',
		]
	}
	]
}

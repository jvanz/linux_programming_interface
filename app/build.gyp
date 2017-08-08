{
	'variables': {
		'gcc_flags': [
			'-Werror -Wall -Wextra -fdiagnostics-color=always'
		]
	},
	'target_defaults':{
		'default_configuration': "Release",
		'configurations': {
			'Debug': {
				'cflags': ['<@(gcc_flags)', '-g'],
				'defines': [
					'LDEBUG'
				]
			},
			'Release': {
				'cflags': ['<@(gcc_flags)']
			}
		}
	},
	'targets': [
	{
		'target_name': 'logger',
		'type': 'executable',
		'sources': [
			'src/logger.c'
		]
	},
	{
		'target_name': 'dummy',
		'type': 'executable',
		'sources': [
			'src/dummy.c'
		]
	}
	]
}

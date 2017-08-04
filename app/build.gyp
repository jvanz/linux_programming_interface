{
	'variables': {
		'gcc_flags': [
			'-Werror -Wall -Wextra'
		]
	},
	'target_defaults':{
		'default_configuration': "Release",
		'configurations': {
			'Debug': {
				'cflags': ['<@(gcc_flags)', '-g'],
				'defines': [
					'DEBUG'
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
	}
	]
}

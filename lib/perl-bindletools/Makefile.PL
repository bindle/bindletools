# -*- perl -*-

use inc::Module::Install;

perl_version    5.008001;

name            'perl-bindletools';
module_name     'Bindle::CGI';
abstract        'Misc Perl functions';
version_from    'lib/Bindle/Version.pm';
author          'David M. Syzdek <syzdek@bindlebinaries.com>';
license         'bsd';

resources
	homepage    => '',
	repository  => '',
	bugtracker  => '',
	MailingList => '',
	ListArchive => '';

requires	'strict' => 1.02;

test_requires	'strict' => 1.02;

features
	'IPv6 support' => [
		-default => 0,
		'IO::Socket::INET6' => 0,
	];

auto_install_now;

WriteAll	sign => 1;


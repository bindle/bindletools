package Bindle::CGI;
#
#  Bindle Binaries Tools
#  Copyright (c) 2012 Bindle Binaries <syzdek@bindlebinaries.com>.
#  All rights reserved.
#
#  @BINDLE_BINARIES_BSD_LICENSE_START@
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are
#  met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of Bindle Binaries nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
#  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BINDLE BINARIES BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
#  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
#  SUCH DAMAGE.
#
#  @BINDLE_BINARIES_BSD_LICENSE_END@
#

# +-=-=-=-=-=-+
# |           |
# |  Headers  |
# |           |
# +-=-=-=-=-=-+

use                     5.008;
use CGI::Carp           qw(fatalsToBrowser);
use Scalar::Util        qw(tainted blessed);
use strict;
use warnings;

use Bindle::Version;

require Exporter;

our @ISA                = qw(Exporter);
our @EXPORT_OK          = qw();
our %EXPORT_TAGS        = qw();
our @EXPORT             = qw();
our $VERSION            = $Bindle::Version::VERSION;

our $ERR                = 0;
our $ERRSTR             = "";


# +-=-=-=-=-=-=-=+
# |              |
# |  Prototypes  |
# |              |
# +-=-=-=-=-=-=-=+

sub get_parsed_env();
sub get_parsed_http_cookie();
sub get_parsed_path_info();
sub get_parsed_post_data();
sub get_parsed_query_string();
sub init(%);
sub parse_env(;$);
sub parse_http_cookie(;$);
sub parse_path_info(;$$);
sub parse_post(;$);
sub parse_query_string(;$);
sub test_cgi(;$);
sub test_css();
sub test_form_fields();


# +-=-=-=-=-=-=-+
# |             |
# |  Functions  |
# |             |
# +-=-=-=-=-=-=-+

sub get_parsed_env()
{
   my $self = shift;
   my $ref;
   $ref = $self->{'env'};
   return(%$ref);
}


sub get_parsed_http_cookie()
{
   my $self = shift;
   my $ref;
   $ref = $self->{'http_cookie'};
   return(%$ref);
}


sub get_parsed_path_info()
{
   my $self = shift;
   my $ref;
   $ref = $self->{'path_info'};
   return(@$ref);
}


sub get_parsed_post_data()
{
   my $self = shift;
   my $ref;
   $ref = $self->{'post_data'};
   return(%$ref);
}


sub get_parsed_query_string()
{
   my $self = shift;
   my $ref;
   $ref = $self->{'query_string'};
   return(%$ref);
}


sub init(%)
{
   # Grabs passed vars
   my $package  = shift;
   my %data     = %_;

   # declares local vars
   my $self;
   my $env;
   my $ref;
   my $key;
   my $count;

   # Makes blessed hash
   $self              = bless({}, $package);
   $self->{'package'} = $package;

   # verifies environment
   $count = keys(%data);
   if ($count < 1)
   {
      $env = \%$ENV;
   } else {
      $env = \%data;
   };

   # checks for taint mode
   if (!(tainted($ENV{'PATH'})))
   {
      die "Must use taint mode with $package\n";
   };

   # parses HTTP data
   $self->{'env'}           = parse_env($env);
   $self->{'post_data'}     = parse_post($env);
   $self->parse_query_string($env->{'QUERY_STRING'});
   $self->parse_http_cookie($env->{'HTTP_COOKIE'});
   $self->parse_path_info($env->{'PATH_INFO'});

   # combines form data sources
   $self->{'form'} = {};
   $ref = $self->{'cookie'};
   foreach $key (keys(%$ref))
   {
      $self->{'form'}->{$key} = $ref->{$key};
   };
   $ref = $self->{'query_string'};
   foreach $key (keys(%$ref))
   {
      $self->{'form'}->{$key} = $ref->{$key};
   };
   $ref = $self->{'post_data'};
   foreach $key (keys(%$ref))
   {
      $self->{'form'}->{$key} = $ref->{$key};
   };

   return($self);
};


sub parse_env(;$)
{
   # grabs passed args
   my $self = shift;
   my $env  = shift || \%ENV;

   # declares local vars
   my $data;
   my $key;

   $data = {};

   # parses environment variables
   foreach $key (keys(%$env))
   {
      if ($key =~ /^([\w]+)$/)
      {
         $key = $1;
         $data->{$key} = $env->{$key};
      };
   };

   # saves parsed data
   if ((blessed($self)))
   {
      $self->{'env'} = $data;
   };

   return($data);
}


sub parse_http_cookie(;$)
{
   # grabs passed args
   my $self   = shift;
   my $string = shift || $ENV{'HTTP_COOKIE'};

   # declares local vars
   my $data;
   my $pair;
   my $name;
   my $value;
   my @pairs;

   $data = {};

   # exits if nothing to do
   if (!(defined($string)))
   {
      if ((blessed($self)))
      {
         $self->{'http_cookie'} = $data;
      };
      return($data);
   };

   # loops through vars and displays unformatted data
   $string =~ s/; /;/g;
   @pairs = split(/;/, $string);
   foreach $pair (@pairs)
   {
      ($name, $value) = split(/=/, $pair);
      if ($name =~ /^([\w]+)$/)
      {
         $name = $1;
         $value =~ tr/+/ /;
         $value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
         $data->{$name} = $value;
      };
   };

   # saves parsed data
   if ((blessed($self)))
   {
      $self->{'http_cookie'} = $data;
   };

   return($data);
};


sub parse_path_info(;$$)
{
   # grabs passed args
   my $self   = shift;
   my $string = shift || $ENV{'PATH_INFO'};
   my $strip  = shift || "/";

   # declares local vars
   my $data = [];
   my $key;

   # exits if nothing to do
   if (!(defined($string)))
   {
      if ((blessed($self)))
      {
         $self->{'path_info'} = $data;
      };
      return($data);
   };

   # adjusts strip string
   if ($string !~ /\/$/)
   {
      $string = $string . '/';
   };
   if ($string !~ /^\//)
   {
      $string = '/' . $string;
   };
   if ($strip !~ /\/$/)
   {
      $strip = $strip . '/';
   };
   if ($strip !~ /^\//)
   {
      $strip = '/' . $strip;
   };

   # parses path info
   $string =~ s/^$strip//g;
   foreach $key (split(/\//, $string))
   {
      if ($key =~ /^([-\w\.]+)$/)
      {
         $data->[@$data] = $key;
      } else {
         $data->[@$data] = '';
      };
   };

   # saves parsed data
   if ((blessed($self)))
   {
      $self->{'path_info'} = $data;
   };

   return($data);
}


sub parse_post(;$)
{
   # grabs passed args
   my $self = shift;
   my $env  = shift || \%ENV;

   # declares local vars
   my $request_method;
   my $content_length;
   my $buffer;
   my $name;
   my $value;
   my $pair;
   my @pairs;
   my $data;

   $data           = {};
   $request_method = $env->{'REQUEST_METHOD'};
   $content_length = $env->{'CONTENT_LENGTH'};

   # verifies post data is available
   if ( (!(defined($request_method))) || (!(defined($content_length))) )
   {
      return($data);
   };
   if ($request_method ne 'POST')
   {
      return($data);
   };

   # parses post data
   read(STDIN, $buffer, $content_length);                   # retrieves posted data
   @pairs = split(/&/, $buffer);                            # splits apart the buffer in name and value pairs
   chomp (@pairs);                                          # removes unnecessary newlines and carriage returns
   foreach $pair (@pairs)                                   # loops through each pair
   {
      ($name, $value) = split(/=/, $pair);                  # seperates the field name and value
      if ($name =~ /^([\w]+)$/)                             # verifies the name has allowed characters
      {
         $name = $1;                                        # detaints data
         $value =~ tr/+/ /;                                           # decodes white space
         $value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg; # decodes hex data
         $data->{$name} = $value;                                     # stores data in a hash
      };
   };

   return($data);
}


sub parse_query_string(;$)
{
   # grabs passed args
   my $self   = shift;
   my $string = shift || $ENV{'QUERY_STRING'};

   # declares local vars
   my $data;
   my $pair;
   my $name;
   my $value;
   my @pairs;

   $data = {};

   # exits if nothing to do
   if (!(defined($string)))
   {
      if ((blessed($self)))
      {
         $self->{'query_string'} = $data;
      };
      return($data);
   };

   # loops through vars
   @pairs = split(/&/, $string);
   foreach $pair (@pairs)
   {
      ($name, $value) = split(/=/, $pair);
      if ($name =~ /^([\w]+)$/)
      {
         $name = $1;
         $value =~ tr/+/ /;
         $value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
         $data->{$name} = $value;
      };
   };

   # saves parsed data
   if ((blessed($self)))
   {
      $self->{'query_string'} = $data;
   };

   return($data);
}


sub test_cgi(;$)
{
   # Grabs passed vars
   my $self   = shift;
   my $prefix = shift || "";

   # declares local vars
   my $cgi;
   my $key;
   my @path_info;
   my %env;
   my %http_cookie;
   my %post_data;
   my %query_string;
   my %fields;

   # initializes CGI framework
   $cgi = $self;
   if (!(blessed($self)))
   {
      $cgi = Bindle::CGI->init(%ENV);
   };

   # munges path_info
   $cgi->parse_path_info($cgi->{'env'}->{'PATH_INFO'}, $prefix);
   @path_info = $cgi->get_parsed_path_info();

   # returns CSS for test CGI
   if ((defined($path_info[0])))
   {
      if ($path_info[0] =~ /^style.css$/)
      {
         return(Bindle::CGI->test_css());
      };
   };

   # defines standard test form fields
   %fields = Bindle::CGI->test_form_fields();

   # retrieve parsed data
   %env          = $cgi->get_parsed_env();
   %http_cookie  = $cgi->get_parsed_http_cookie();
   %post_data    = $cgi->get_parsed_post_data();
   %query_string = $cgi->get_parsed_query_string();

   # resets cookie data
   foreach $key (sort(keys(%http_cookie)))
   {
      printf("Set-Cookie: %s==null; path=/; Max-Age=0; expires=Tue, 15 Jan 2013 01:01:01 GMT;\n", $key);
   };
   foreach $key (keys(%fields))
   {
      printf("Set-Cookie: %s=%s; path=/; Max-Age=3600; Domain=.%s;\n", $key, $fields{$key}, $ENV{'HTTP_HOST'});
   };
   printf("Set-Cookie: dyn%s=%s; path=/; Max-Age=3600; Domain=.%s;\n", int(rand(999999)), rand(), $ENV{'HTTP_HOST'});

   # prints HTTP headers
   printf("Content-type: text/html; charset=UTF-8\n");
   printf("Cache-Control: no-cache\n");
   printf("Last-Modified: %s\n", localtime());
   printf("\n");

   # prints top HTML
   printf("<!DOCTYPE HTML>\n");
   printf("<html>\n");
   printf("  <head>\n");
   printf("    <title>Test CGI</title>\n");
   printf("    <link href='%s/%s/style.css' type='text/css' rel='stylesheet' />\n", $ENV{'SCRIPT_NAME'}, $prefix);
   printf("  </head>\n");
   printf("  <body>\n");
   printf("    <div id='wrapper'>\n");
   printf("      <div id='left'>\n");
   printf("        <h3>Test CGI</h3>\n");
   printf("        <ul class='submenu'>\n");
   printf("        <li><a href='#environment'>Web Server Variables</a></li>\n");
   printf("        <li><a href='#path_info'>PATH_INFO Nodes</a></li>\n");
   printf("        <li><a href='#query_string'>Query String Data</a></li>\n");
   printf("        <li><a href='#post_data'>POST Data</a></li>\n");
   printf("        <li><a href='#http_cookie'>HTTP Cookie Data</a></li>\n");
   printf("        </ul>\n");
   printf("      </div><!-- #left (end) -->\n");
   printf("      <div id='right'>\n");
   printf("      </div><!-- #right (end) -->\n");
   printf("      <div id='content'>\n");

   # prints parsed ENV
   print ("        <a name='environment'></a>\n");
   print ("        <h3>Web Server Variables</h3>\n");
   printf("        <p>\n");
   print ("          <table>\n");
   print ("            <tr><td><b>Variable</b></td><td><b>Value</b></td></tr>\n");
   foreach $key (sort(keys(%env)))
   {
      printf ("            <tr><td>%s</td><td>%s</td></tr>\n", $key, $env{$key});
   };
   print ("          </table>\n");
   printf("        </p>\n");

   # prints PATH_INFO
   print ("        <a name='path_info'></a>\n");
   print ("        <h3>PATH_INFO nodes</h3>\n");
   printf("        <p>\n");
   print ("          <ul>\n");
   foreach $key (@path_info)
   {
      printf ("            <li>%s</li>\n", $key);
   };
   print ("          </ul>\n");
   printf("        </p>\n");
   printf("        <p>\n");
   printf("          <table>\n");
   printf("            <tr>\n");
   printf("              <td>\n");
   printf("                <form method=link action=%s%s", $ENV{'SCRIPT_NAME'}, $prefix);
   foreach $key (sort(keys(%fields)))
   {
      printf("/%s", $key);
   };
   printf(">\n");
   printf("                  <input type=submit value='Set PATH_INFO'>\n");
   printf("                </form>\n");
   printf("              </td>\n");
   printf("            </tr>\n");
   printf("          </table>\n");
   printf("        </p>\n");

   # prints parsed HTTP_COOKIE
   print ("        <a name='http_cookie'></a>\n");
   print ("        <h3>HTTP_COOKIE Data</h3>\n");
   printf("        <p>\n");
   print ("          <table>\n");
   print ("            <tr><td><b>Field</b></td><td><b>Value</b></td></tr>\n");
   foreach $key (sort(keys(%http_cookie)))
   {
      printf ("            <tr><td>%s</td><td>%s</td></tr>\n", $key, $http_cookie{$key});
   };
   print ("          </table>\n");
   printf("        </p>\n");

   # prints parsed QUERY_STRING
   print ("        <a name='query_string'></a>\n");
   print ("        <h3>QUERY_STRING Data</h3>\n");
   printf("        <p>\n");
   print ("          <table>\n");
   print ("            <tr><td><b>Field</b></td><td><b>Value</b></td></tr>\n");
   foreach $key (sort(keys(%query_string)))
   {
      printf ("            <tr><td>%s</td><td>%s</td></tr>\n", $key, $query_string{$key});
   };
   print ("          </table>\n");
   printf("        </p><p>\n");
   printf("          <table>\n");
   printf("            <tr>\n");
   printf("              <td>\n");
   printf("                <form method=link>\n");
   foreach $key (sort(keys(%fields)))
   {
      printf("                  <input type=hidden name='%s' value='%s'>\n", $key, $fields{$key});
   };
   printf("                  <input type=submit value='Set QUERY_STRING'>\n");
   printf("                </form>\n");
   printf("              </td>\n");
   printf("            </tr>\n");
   printf("          </table>\n");
   printf("        </p>\n");

   # prints parsed POST data
   print ("        <a name='post_data'></a>\n");
   print ("        <h3>POST Data</h3>\n");
   printf("        <p>\n");
   print ("          <table>\n");
   print ("            <tr><td><b>Field</b></td><td><b>Value</b></td></tr>\n");
   foreach $key (sort(keys(%post_data)))
   {
      printf ("            <tr><td>%s</td><td>%s</td></tr>\n", $key, $post_data{$key});
   };
   print ("          </table>\n");
   printf("        </p><p>\n");
   printf("          <table>\n");
   printf("            <tr>\n");
   printf("              <td>\n");
   printf("                <form method=post>\n");
   foreach $key (keys(%fields))
   {
      printf("                  <input type=hidden name='%s' value='%s'>\n", $key, $fields{$key});
   };
   printf("                  <input type=submit value='Set POST data'>\n");
   printf("                </form>\n");
   printf("              </td>\n");
   printf("            </tr>\n");
   printf("          </table>\n");
   printf("        </p>\n");

   # prints bottom HTML
   printf("      </div><!-- #content (end) -->\n");
   printf("    </div><!-- #wrapper (end) -->\n");
   printf("  </body>\n");
   printf("</html>\n");

   return(0);
}


sub test_css()
{

   printf("Content-type: text/css; charset=UTF-8\n");
   printf("\n");

   print <<EOF;


body
{
	margin:      0 0 0 0;
	padding:     0 0 0 0;
	background:  #FFFFFF;
	color:       #666666;
	font-family: verdana, arial, helvetica, sans-serif;
	font-size:   11px;
}

a:link    { color: #5b8fbe; text-decoration: none; }
a:visited { color: #5b8fbe; text-decoration: none; }
a:active  { color: #5b8fbe; text-decoration: none; }
a:hover   { color: #5b8fbe; text-decoration: underline; }

#wrapper
{
	min-height: 300px;
	margin:     20px auto 0 auto;
	padding:    20px 0 0 0;
}

#wrapper h3
{
	margin:      10px 0 0 0;
	color:       #5b8fbe;
}

#wrapper pre
{
	margin:   0 0 0 0;
	padding:  0 5px 0 5px;
	background: #cccccc;
	color:      #000000;
}

#left
{
	float:   left;
	width:   140px;
	margin:  0 15px 0 15px;
	border:  0;
	padding: 0 0 20px 0;
}

#right
{
	float:   right;
	width:   140px;
	margin:  0 15px 0 15px;
	border:  0;
	padding: 0 0 20px 0;

}

#content
{
	margin:  0 185px 0 185px;
	border:  0;
	padding: 0 0 20px 0;
	font-size:   11px;
}

.submenu
{
	margin:        0 0 20px 0;
	border-bottom: 1px solid #D1D9C4;
	padding:       0 0 0 0;
}

.submenu li
{
	margin:      0 0 0 0;
	border-top:  1px solid #D1D9C4;
	line-height: 20px;
	font-size:   10px;
	list-style:  none;
}

EOF

   return(0);
};


sub test_form_fields()
{
   my $package  = shift;
   my %fields;

   $fields{'date'}           = localtime();
   $fields{'program_name'}   = $ENV{'SCRIPT_NAME'};
   $fields{'package'}        = $package;
   $fields{'version'}        = $Bindle::CGI::VERSION;
   $fields{'random_number'}  = rand();

   return(%fields);
}


1;
# end of module

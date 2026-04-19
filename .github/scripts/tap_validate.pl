#!/usr/bin/env perl
# Validate that a test executable produces well-formed TAP version 13 output.
#
# Usage: tap_validate.pl <test_executable>

use strict;
use warnings;
use TAP::Parser;

my $exe = shift or die "Usage: $0 <test_executable>\n";

my $parser = TAP::Parser->new({ exec => [$exe, '-ptap'] });
while ($parser->next) {}

my @errors = $parser->parse_errors;
if (@errors) {
    print "TAP parse error: $_\n" for @errors;
    exit 1;
}

print "TAP output is valid.\n";

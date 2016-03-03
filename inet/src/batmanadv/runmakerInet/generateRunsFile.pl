#!/usr/bin/env perl

use List::Util qw(shuffle);

my $command = "./run -u Cmdenv -c";

my @configs = `./run -a`;
my @runs = ();

foreach (@configs) {
	if ($_ =~ /Config ([^\:]*): (\d*)/) {
		for (my $i=0; $i < $2; $i++) {
			push(@runs,". $command $1 -r $i\n");
		}
	}
}

print $_ foreach(shuffle @runs);

#!/usr/bin/perl -w
use strict;
use JSON;

my $LEVEL = 1;

my $level = 
[
	{
		type => "exit",
		width => 25,
		height => 50,
		x => 100,
		y => 325
	},
	{
		type => "exit",
		width => 25,
		height => 50,
		x => 300,
		y => 325
	},
	{
		type => "static",
		width => 600,
		height => 25,
		x => 0,
		y => 375
	},
	{
		type => "start",
		pos => 
		[
			{
				x => 200,
				y => 100
			},
			{
				x => 300,
				y => 150
			}
		]
	}
];

my $filename = "assets/levels/level$LEVEL.json";
open my $file, '>', $filename or die "Cannot open '$filename': $!\n";
print $file to_json($level, { pretty => 1 });

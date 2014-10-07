#!/usr/bin/perl -w
use strict;
use JSON;

my $levels = 
[
	[
		1,
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
			x => 350,
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
			type => "static",
			width => 25,
			height => 600,
			x => 287.5,
			y => 0
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
	],
];

#Write the levels to JSON:
for my $level(@$levels)
{
	my $levelnum = shift @$level;
	my $filename = "assets/levels/level$levelnum.json";
	open my $file, '>', $filename or die "Cannot open '$filename': $!\n";
	print $file to_json($level, { pretty => 1 });
}

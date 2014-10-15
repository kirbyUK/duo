#!/usr/bin/perl -w
use strict;
use JSON;

my $levels = 
[
	[
		1,
		# The two start positions:
		{
			type => "start",
			pos => 
			[
				{
					x => 235,
					y => 175
				},
				{
					x => 550,
					y => 175
				}
			]
		},
		# The two exits:
		{
			type => "exit",
			width => 25,
			height => 50,
			x => 235,
			y => 325
		},
		{
			type => "exit",
			width => 25,
			height => 50,
			x => 550,
			y => 325
		},
		# The two mandatory static blocks:
		{
			type => "static",
			width => 600,
			height => 25,
			x => 0,
			y => 375
		},
		{
			type => "static",
			width => 30,
			height => 600,
			x => 285,
			y => 0
		},
		# Any extra blocks:
		{
			type => "static",
			width => 100,
			height => 25,
			x => 185,
			y => 200
		},
		{
			type => "static",
			width => 100,
			height => 25,
			x => 500,
			y => 200
		},
		# Additional objects:
		{
			type => "arrow",
			facing => "south",
			x => 247,
			y => 315
		},
		{
			type => "arrow",
			facing => "south",
			x => 562,
			y => 315
		}
	],
	[
		2,
		# The two start positions:
		{
			type => "start",
			pos => 
			[
				{
					x => 20,
					y => 350,
				},
				{
					x => 320,
					y => 140,
				}
			]
		},
		# The two exits:
		{
			type => "exit",
			width => 25,
			height => 50,
			x => 235,
			y => 145
		},
		{
			type => "exit",
			width => 25,
			height => 50,
			x => 550,
			y => 325
		},
		# The two mandatory static blocks:
		{
			type => "static",
			width => 600,
			height => 25,
			x => 0,
			y => 375
		},
		{
			type => "static",
			width => 30,
			height => 600,
			x => 285,
			y => 0
		},
		# Other blocks:
		# Left side:
		{
			type => "static",
			width => 100,
			height => 20,
			x => 185,
			y => 315,
		},
		{
			type => "static",
			width => 100,
			height => 20,
			x => 0,
			y => 255,
		},
		{
			type => "static",
			width => 100,
			height => 20,
			x => 185,
			y => 195,
		},
		# Right side:
		{
			type => "static",
			width => 100,
			height => 20,
			x => 315,
			y => 165,
		},
		{
			type => "static",
			width => 100,
			height => 20,
			x => 500,
			y => 245,
		},
		{
			type => "static",
			width => 100,
			height => 20,
			x => 315,
			y => 345,
		},
	],
	[
		3,
		# The two start positions:
		{
			type => "start",
			pos => 
			[
				{
					x => 20,
					y => 325,
				},
				{
					x => 320,
					y => 325,
				}
			]
		},
		# The two exits:
		{
			type => "exit",
			width => 50,
			height => 50,
			x => 225,
			y => 325
		},
		{
			type => "exit",
			width => 50,
			height => 50,
			x => 325,
			y => 325
		},
		# The two mandatory static blocks:
		{
			type => "static",
			width => 600,
			height => 25,
			x => 0,
			y => 375
		},
		{
			type => "static",
			width => 30,
			height => 600,
			x => 285,
			y => 0
		},
		# Other blocks
		# Left side:
		{
			type => "static",
			width => 15,
			height => 50,
			x => 170,
			y => 325,
		},
	],
	[
		4,
		# The two start positions:
		{
			type => "start",
			pos => 
			[
				{
					x => 20,
					y => 125,
				},
				{
					x => 320,
					y => 125,
				}
			]
		},
		# The two exits:
		{
			type => "exit",
			width => 50,
			height => 50,
			x => 125,
			y => 325
		},
		{
			type => "exit",
			width => 25,
			height => 50,
			x => 560,
			y => 100
		},
		# The two mandatory static blocks:
		{
			type => "static",
			width => 600,
			height => 25,
			x => 0,
			y => 375
		},
		{
			type => "static",
			width => 30,
			height => 600,
			x => 285,
			y => 0
		},
		# Other blocks
		# Left side:
		{
			type => "static",
			button =>
			{
				type => "pressure",
				x => 430,
				y => 135,
				blockx => 0,
				blocky => 150,
			},
			width => 50,
			height => 25,
			x => 125,
			y => 150,
		},
		{
			type => "static",
			width => 125,
			height => 250,
			x => 0,
			y => 150,
		},
		{
			type => "static",
			width => 125,
			height => 250,
			x => 175,
			y => 150,
		},
		# Right side:
		{
			type => "static",
			width => 300,
			height => 30,
			x => 300,
			y => 150
		}
	],
	[
		5,
		# The two start positions:
		{
			type => "start",
			pos => 
			[
				{
					x => 20,
					y => 175,
				},
				{
					x => 320,
					y => 175,
				}
			]
		},
		# The two exits:
		{
			type => "exit",
			width => 25,
			height => 50,
			x => 235,
			y => 25
		},
		{
			type => "exit",
			width => 25,
			height => 50,
			x => 575,
			y => 25
		},
		# The two mandatory static blocks:
		{
			type => "static",
			width => 600,
			height => 25,
			x => 0,
			y => 375
		},
		{
			type => "static",
			width => 30,
			height => 600,
			x => 285,
			y => 0
		},
		# Other blocks
		# Left side:
		{
			type => "static",
			width => 300,
			height => 20,
			x => 0,
			y => 200
		},
		{
			type => "static",
			width => 75,
			height => 25,
			x => 210,
			y => 75
		},
		{
			type => "static",
			width => 75,
			height => 25,
			x => 0,
			y => 75
		},
		{
			type => "static",
			button =>
			{
				type => "toggle",
				x => 530,
				y => 170,
				blockx => 75,
				blocky => 137.5
			},
			width => 135,
			height => 25,
			x => 75,
			y => 75,
		},
		# Right side:
		{
			type => "static",
			width => 300,
			height => 20,
			x => 300,
			y => 200
		},
		{
			type => "static",
			width => 75,
			height => 25,
			x => 315,
			y => 75
		},
		{
			type => "static",
			width => 75,
			height => 25,
			x => 525,
			y => 75
		},
		{
			type => "static",
			width => 25,
			height => 15,
			x => 530,
			y => 185,
		},
		{
			type => "static",
			button =>
			{
				type => "toggle",
				x => 40,
				y => 60,
				blockx => 390,
				blocky => 137.5
			},
			width => 135,
			height => 25,
			x => 390,
			y => 75,
		},
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

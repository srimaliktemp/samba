#
# mksyms.awk
#
# Extract symbols to export from C-header files.
# output in version-script format for linking shared libraries.
#
# Copyright (C) 2008 Micheal Adam <obnox@samba.org>
#
BEGIN {
	inheader=0;
	current_file="";
	print "#"
	print "# This file is automatically generated with \"make symbols\". DO NOT EDIT "
	print "#"
	print "{"
	print "\tglobal:"
}

END {
	print""
	print "\tlocal: *;"
	print "};"
}

{
	if (FILENAME!=current_file) {
		print "\t\t# The following definitions come from",FILENAME
		current_file=FILENAME
	}
	if (inheader) {
		if (match($0,"[)][^()]*[;][ \t]*$")) {
			inheader = 0;
		}
		next;
	}
}

/^static/ || /^[ \t]*typedef/ || !/^[a-zA-Z\_]/ {
	next;
}

/^extern[ \t]+[^()]+[;][ \t]*$/ {
	gsub(/[^ \t]+[ \t]+/, "");
	sub(/[;][ \t]*$/, "");
	printf "\t\t%s;\n", $0;
	next;
}

# look for function headers:
{
	gotstart = 0;
	if ($0 ~ /^[A-Za-z_][A-Za-z0-9_]+/) {
	gotstart = 1;
	}
	if(!gotstart) {
		next;
	}
}

/[_A-Za-z0-9]+[ \t]*[(].*[)][^()]*;[ \t]*$/ {
	sub(/[(].*$/, "");
	gsub(/[^ \t]+[ \t]+/, "");
	gsub(/^[*]+/, "");
	printf "\t\t%s;\n",$0;
	next;
}

/[_A-Za-z0-9]+[ \t]*[(]/ {
	inheader=1;
	sub(/[(].*$/, "");
	gsub(/[^ \t]+[ \t]+/, "");
	gsub(/^[*]/, "");
	printf "\t\t%s;\n",$0;
	next;
}


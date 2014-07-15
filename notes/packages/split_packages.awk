BEGIN {
	RS="\n\n";
	FS=" ";
}
{
	name=$2;
	print > name;
	close(name);
}



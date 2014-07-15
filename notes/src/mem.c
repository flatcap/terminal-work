#include <stdio.h>
#include <stdlib.h>

const char spaces[] = "                                                                                ";

int indent_stack (void)
{
	const int scale = 4;

	if (!__builtin_frame_address(0))
		return scale*0;
	if (!__builtin_frame_address(1))
		return scale*0;
	if (!__builtin_frame_address(2))
		return scale*0;
	if (!__builtin_frame_address(3))
		return scale*1;
	if (!__builtin_frame_address(4))
		return scale*2;
	if (!__builtin_frame_address(5))
		return scale*3;
	if (!__builtin_frame_address(6))
		return scale*4;
	if (!__builtin_frame_address(7))
		return scale*5;
	if (!__builtin_frame_address(8))
		return scale*6;
	if (!__builtin_frame_address(9))
		return scale*7;
	return scale*8;
}

void dave (void)
{
	printf ("Entering %.*s%s\n", indent_stack(), spaces, __FUNCTION__);
}

void bob (void)
{
	printf ("Entering %.*s%s\n", indent_stack(), spaces, __FUNCTION__);
	dave();
}

void jim (void)
{
	printf ("Entering %.*s%s\n", indent_stack(), spaces, __FUNCTION__);
	bob();
}

int main (int argc, char *argv[])
{
	printf ("Entering %.*s%s\n", indent_stack(), spaces, __FUNCTION__);
	jim();

	return 0;
}

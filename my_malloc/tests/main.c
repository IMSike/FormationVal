int main(int argc, char const *argv[])
{
	char *test= mon_malloc(2*sizeof(char));
	mon_free(test);
}
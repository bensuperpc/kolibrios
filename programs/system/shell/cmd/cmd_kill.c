
int cmd_kill(char param[])
{
	unsigned process;
	int result;
	int i;

	if ( strlen(param) == 0 ) {
		printf(CMD_KILL_USAGE);
		return TRUE;
	}

	if (!strcmp(param, "all")) {
		for (i = 2;i<256;i++) {
			kol_kill_process(i);
		}
		return TRUE;
	}
	else {
		process = atoi(param);
		if ( 0 != process ) {
			result = kol_process_kill_pid(process);
			if (result < 0)
				return FALSE;
			else
				return TRUE;
		}
	}

}


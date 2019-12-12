/*
 * Cli.c
 *
 *  Created on: Dec 11, 2019
 *      Author: TBiberdorf
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define CLI_BUFFER_SIZE (128)


typedef struct _cliCommandOptions_s
{
	const char *option;
	int32_t (*optionCallback)(char *);
}s_cliCommandOptions_t;

typedef struct _cliCommands_s
{
	const char *cmdString;
	s_cliCommandOptions_t *option;
}s_cliCommands_t;


/*
 * parse InputStr for decimal value.
 * @return ptrEnd will be null if no more data to parse
 */
static uint8_t * ParseDecimal(uint8_t *InputStr, uint32_t *PtrDecimal)
{
	int16_t idx;
	uint8_t *ptrStart;
	uint8_t *ptrEnd = NULL;
	uint32_t value;
	ptrStart = InputStr;
	idx = 0;

	/*
	 * first look for the first digit of the decimal number
	 */
	while(*(ptrStart+idx) == ' ')
	{
		idx++;
		/*
		 * @todo
		 * may want to put a check here, to make sure the idx does not
		 * get too big
		 */
	}
	ptrStart = ptrStart+idx;
	value = strtoul((char*)ptrStart,(char **)&ptrEnd,10);
	*PtrDecimal = value;

	if(ptrEnd == NULL)
	{
		/*
		 * no more data to parse
		 */
	}
	return ptrEnd;
}



/*
 * parse InputStr for hex value.
 * @return ptrEnd will be null if no more data to parse
 */
static uint8_t * ParseHex(uint8_t *InputStr, uint32_t *PtrHex)
{
	int16_t idx;
	uint8_t *ptrStart;
	uint8_t *ptrEnd = NULL;
	uint32_t value;
	ptrStart = InputStr;
	idx = 0;

	/*
	 * first look for the first digit of the decimal number
	 */
	while(*(ptrStart+idx) == ' ')
	{
		idx++;
		/*
		 * @todo
		 * may want to put a check here, to make sure the idx does not
		 * get too big
		 */
	}
	ptrStart = ptrStart+idx;
	value = strtoul((char*)ptrStart,(char **)&ptrEnd,16);
	*PtrHex = value;

	if(ptrEnd == NULL)
	{
		/*
		 * no more data to parse
		 */
	}
	return ptrEnd;
}

static int32_t helpCmd(char *Param)
{
	printf("NGR help commands:\r\n");
	return 0;
}

s_cliCommandOptions_t helpOptions[]= {
		{"-h",helpCmd},{"-?",helpCmd},{NULL,helpCmd}
};


static int32_t gpioPinSelect(char *Param)
{
	printf("pin select\r\n");
	return 0;
}


static int32_t gpioCmdHelp(char *Param)
{
	printf("GPIO Help commands:\r\n");
	printf("-p select pin:\r\n");
	printf("-s set the selected pin:\r\n");
	printf("-c clear the selected pin:\r\n");
	return 0;
}

s_cliCommandOptions_t gpioOptions[]= {
		{"-p",gpioPinSelect},
		{"-h",gpioCmdHelp},{"-?",gpioCmdHelp},{NULL,gpioCmdHelp}
};

s_cliCommands_t userCmds[]= {
		{"help" ,helpOptions},
		{"gpio" ,gpioOptions},
		{NULL,NULL}
};

static int32_t LoopOptions(s_cliCommands_t *UserCmdOptions,uint8_t *PtrOption)
{
	int8_t idx;
	int8_t optionSize=0;
	int32_t status = -1;

	for(idx=0; UserCmdOptions->option[idx].option != NULL;)
	{
		optionSize = strlen(UserCmdOptions->option[idx].option);
		if( strncmp((char *)PtrOption,UserCmdOptions->option[idx].option,optionSize) == 0)
		{
			/*
			 * we have a matching option and the pointer to where the parameter
			 * will be located.
			 */

			/*
			 * verify we have a method to call
			 */
			if( UserCmdOptions->option[idx].optionCallback != 0)
			{
				status = UserCmdOptions->option[idx].optionCallback((char *)(PtrOption+optionSize));
			}
		}
		idx++;
	}

	/*
	 * for commands that have no options
	 */
	if( status < 0 )
	{
		/*
		 * verify we have a method to call
		 */
		if( UserCmdOptions->option[idx].optionCallback != 0)
		{
			status = UserCmdOptions->option[idx].optionCallback((char *)(PtrOption+optionSize));
		}
	}


	return status;
}

static int32_t LoopCmds(uint8_t *EnteredCmd)
{
	int32_t status = -1;
	int32_t cmdStringLength;
	uint8_t *ptrOption = NULL;
	s_cliCommands_t *ptrUserCmds = &userCmds[0];
	while( ptrUserCmds->cmdString != NULL)
	{
		cmdStringLength = strlen(ptrUserCmds->cmdString);
		if( strncmp((char *)EnteredCmd,ptrUserCmds->cmdString,cmdStringLength) == 0)
		{
			if(*(EnteredCmd+cmdStringLength) == '+')
			{
				ptrOption = EnteredCmd+cmdStringLength;
			}
			else
			{
				ptrOption = EnteredCmd+cmdStringLength+1;
			}
			status = LoopOptions(ptrUserCmds,ptrOption);
		}
		ptrUserCmds++;
	}

	if(status < 0)
	{
		printf("unknown command\r\n");
	}

	return status;
}


/**
 * @brief entry point for all CLI commands
 * @details Each CLI entry key is queued to a tl_cliData buffer.  This buffer is
 * analyzed to determine if a valid CLI command is present.  If so, then the
 * link list command will be called
 *
 * @param InputKey
 * @return void
 */
void cli(uint8_t InputKey)
{
	static uint8_t tl_cliData[CLI_BUFFER_SIZE];
	static uint16_t tl_cliIdx = 0;

	tl_cliData[tl_cliIdx++] = InputKey;

	if( tl_cliData[tl_cliIdx-1] == '\n')
	{
		tl_cliData[tl_cliIdx-1] = 0;
		if( tl_cliData[tl_cliIdx-2] == '\r')
		{
			tl_cliData[tl_cliIdx-2] = 0;
		}

		tl_cliIdx = 0;
		LoopCmds(tl_cliData);
	}

	if(tl_cliIdx == CLI_BUFFER_SIZE)
	{
		tl_cliData[CLI_BUFFER_SIZE-1] = 0x0;
		tl_cliIdx = 0;
		LoopCmds(tl_cliData);
	}

}


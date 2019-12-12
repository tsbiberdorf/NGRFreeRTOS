/*
 * Cli.c
 *
 *  Created on: Dec 11, 2019
 *      Author: TBiberdorf
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CLI_BUFFER_SIZE (128)

static uint8_t tl_cliData[CLI_BUFFER_SIZE];
static uint16_t tl_cliIdx = 0;

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


int32_t helpCmd(char *Param)
{
	printf("NGR help commands:\r\n");
	return 0;
}

s_cliCommandOptions_t helpOptions[]= {
		{"-h",helpCmd},{"-?",helpCmd},{NULL,helpCmd}
};

s_cliCommands_t userCmds[]= {
		{"help" ,helpOptions},
		{NULL,NULL}
};

int32_t LoopOptions(s_cliCommands_t *UserCmdOptions,uint8_t *PtrOption)
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

int32_t LoopCmds(uint8_t *EnteredCmd)
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

//static int32_t parseInputCLI( uint8_t *Input,int8_t Size)
//{
//	int32_t cmdFound = 0;
//	uint16_t idx;
//	for(idx=0;idx<Size;idx++)
//	{
//		tl_cliData[tl_cliIdx++] = Input[idx];
//
//		if( tl_cliData[tl_cliIdx-1] == '\n')
//		{
//			cmdFound = 1;
//			tl_cliData[tl_cliIdx-1] = 0;
//			tl_cliIdx = 0;
//			LoopCmds(tl_cliData);
//		}
//		if(tl_cliIdx == CLI_BUFFER_SIZE)
//		{
//			tl_cliData[CLI_BUFFER_SIZE-1] = 0x0;
//			tl_cliIdx = 0;
//			LoopCmds(tl_cliData);
//		}
//	}
//	return cmdFound;
//}

void cli(uint8_t InputKey)
{
	tl_cliData[tl_cliIdx++] = InputKey;

	if( tl_cliData[tl_cliIdx-1] == '\n')
	{
		tl_cliData[tl_cliIdx-1] = 0;
		if( tl_cliData[tl_cliIdx-2] == '\n')
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

//
//#define MSG_BUFFER_SIZE (64)
//	static uint8_t msg[MSG_BUFFER_SIZE];
//	static int8_t msgIdx;
//
//	msg[msgIdx++] = InputKey;
//	if( parseInputCLI(msg, msgIdx) )
//	{
//		msgIdx = 0;
//	}
}


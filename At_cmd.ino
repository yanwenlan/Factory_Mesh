#include <string.h>
#define MAX_CMD_LEN 64
#define MAX_PARAM_LEN 256
#define MAX_ARGV_SIZE 64

typedef struct
{
    char cmd[MAX_CMD_LEN + 1];
    char params[MAX_PARAM_LEN + 1];
    int  argc;
    char* argv[MAX_ARGV_SIZE];    
} AT_Command;

// 函数声明
int handle_chmode(const AT_Command *cmd);
int handle_busy(const AT_Command *cmd);
int handle_play(const AT_Command *cmd);
int handle_vol(const AT_Command *cmd);
int handle_test(const AT_Command *cmd);
AT_Command parse_AT_Command(const char *input);


typedef enum at_fm_error_e_ {
    AT_FM_OK = 0,
    AT_FM_ERROR,
    AT_FM_COMMAND_NOT_FOUND,
    AT_FM_PARAM_ERROR,
} AT_FM_ERROR_E;

// const uint8_t *atcmd_err_tbl[] =
//     {
//         "OK",
//         "AT_ERROR",
//         "AT_PARAM_ERROR",
//         "AT_BUSY_ERROR",
//         "AT_TEST_PARAM_OVERFLOW",
//         "AT_NO_CLASSB_ENABLE",
//         "AT_NO_NETWORK_JOINED",
//         "AT_RX_ERROR",
//         "AT_MODE_NO_SUPPORT",
//         "AT_COMMAND_NOT_FOUND",
//         "AT_UNSUPPORTED_BAND",
// };



//Add a standard error code return value
typedef int (*AT_Handler)(const AT_Command *);

typedef struct
{
    const char *cmd;
    AT_Handler handler;
    const char *help;

} AT_HandlerTable;

/*    AT+CHMODE
loop start ------->

+TASK:4
<----------loop end
loop start ------->

+TASK:3
<----------loop end
*/

int handle_chmode(const AT_Command *cmd)
{
  // 申请内存，接收命令返回结果
  uint8_t* temp = (uint8_t*)malloc(50*sizeof(uint8_t));  
  AT_FM_ERROR_E ret;
  if (temp == NULL) { // 申请内存失败
      Serial.printf("Memory allocation failed\n");
      ret =  AT_FM_ERROR;
  }

  if(cmd->argc != 0)
  {
    ret =  AT_FM_PARAM_ERROR;
  }

  if(sd_flag == 0)  // default mode or Bluetooth mode
  {
    // Serial.printf("handle_chmode before read:   %s\r\n",temp);
    // Serial1.println("AT+CHMODE\r\n");
    char tempCommand[MAX_PARAM_LEN + 1];
    strcpy(tempCommand, cmd->cmd);    
    strcat(tempCommand, "\r\n");
    Serial.printf("tempCommand:%s:tempCommand\r\n", tempCommand);
    Serial1.println(tempCommand);
    delay(10);
    Serial1Read(temp);
    // Serial.printf("handle_chmode:    %s\r\n",temp);
    if(strcmp((const char*)temp, "\r\n+TASK:3\r\n")==0) // 蓝牙模式，不改flag
    {
      Serial.printf("Bluetooth Mode, no change sd_flag...\r\n");    
    }
    else if(strcmp((const char*)temp, "\r\n+TASK:4\r\n")==0)  // SD卡模式，修改flag为1
    {
      Serial.printf("change to sd mode success...\r\n");  
      sd_flag=1;
      ret = AT_FM_OK;
    }
    else
    {
      Serial.printf("change mode error...\r\n");
      ret =  AT_FM_ERROR;
    }
  }
  else if(sd_flag == 1) // SD卡模式，不做处理
  {
    Serial.printf("mode is already sd mode...\r\n");
    ret = AT_FM_OK;  
  }
  else
  {
    Serial.printf("something error...\r\n");
    ret =  AT_FM_ERROR;
  }
  
  // Serial.printf("handle_chmode after read:   %s\r\n",temp);
  free(temp);
  *temp = NULL;
  // Serial.printf("handle_chmode after free:   %s\r\n",temp);
  return ret;
}

int handle_busy(const AT_Command *cmd)
{
  // 申请内存，接收命令返回结果
  uint8_t* temp = (uint8_t*)malloc(50*sizeof(uint8_t));  
  AT_FM_ERROR_E ret;
  if (temp == NULL) { // 申请内存失败
      printf("Memory allocation failed\n");
      return AT_FM_ERROR;
  }
  // Serial1.println("AT+BUSY=0\r\n");
  char tempCommand[MAX_PARAM_LEN + 1];
  strcpy(tempCommand, cmd->cmd);
  strcat(tempCommand, "=");
  strcat(tempCommand, cmd->params);
  strcat(tempCommand, "\r\n");
  Serial1.println(tempCommand);
  delay(10);
  Serial1Read(temp);
  Serial.printf("handle_busy temp_start:%s:temp_end\r\n",temp);
  if(strcmp((const char*)temp, "\r\nOK\r\n")==0) // 返回OK，说明配置成功
  {
    ret = AT_FM_OK;
  }
  else
  {
    ret=AT_FM_ERROR;
  }
  free(temp); *temp = NULL;   // 释放动态内存
  return ret;
}

int handle_play(const AT_Command *cmd)
{

}

int handle_vol(const AT_Command *cmd)
{

}

int handle_test(const AT_Command *cmd)
{

}

AT_HandlerTable handler_table[] = {
  {"AT+CHMODE", handle_chmode, "Change Mode"},
  {"AT+BUSY", handle_busy, "set busy mode"},
  {"AT+VOL", handle_vol, "set vol"},
  {"AT+PLAY", handle_play, "play music from sd card"},
  {"AT+TEST", handle_test, "Test command"}
};

AT_Command parse_AT_Command(const char *input)
{
  AT_Command cmd;
  const char *eq_pos = strchr(input, '=');
  if (eq_pos != NULL)
  {
    size_t cmd_len = eq_pos - input;
    /* Gets the length of the argument */
    size_t params_len = strlen(eq_pos + 1);
    memcpy(cmd.cmd, input, cmd_len);
    memcpy(cmd.params, eq_pos + 1, params_len);
    cmd.cmd[cmd_len] = '\0';
    cmd.params[params_len] = '\0';

    // 使用一个局部变量来分割参数，以保持cmd.params的完整性  需要静态变量 保证内存安全性
    static char params_copy[MAX_PARAM_LEN + 1];     
    strcpy(params_copy, cmd.params);

    char *token = strtok(params_copy, ":");
    int arg_index = 0;

    while (token != NULL && arg_index < MAX_ARGV_SIZE)
    {
        cmd.argv[arg_index++] = token;
        token = strtok(NULL, ":");
    }
    cmd.argc = arg_index;
  }
  else
  {
    /* Without the = sign, the whole string is copied */
    strcpy(cmd.cmd, input);
    cmd.params[0] = '\0';
    cmd.argc = 0;
  }

  return cmd;
}

/*
处理AT命令
*/
void process_AT_Command(const char *input)
{
  int ret = AT_FM_ERROR;
  int i;

  AT_Command cmd = parse_AT_Command(input);
  if (strlen(cmd.cmd) == 0) // When the length of the AT command is 0, no processing is performed.
  {
    Serial.printf("\r\n");
    ret = AT_FM_OK;
    return;
  }

  int num_handlers = sizeof(handler_table) / sizeof(handler_table[0]);
  for ( i = 0; i < num_handlers; i++)
  {
    if (strcasecmp(cmd.cmd, handler_table[i].cmd) == 0)
    {
      Serial.printf("\r\n"); // Add \r\n before returning the result of the AT command.
      ret = handler_table[i].handler(&cmd);
      break;
    }
  }

  if(i == num_handlers)
  {
      ret = AT_FM_COMMAND_NOT_FOUND;
  }

  // if (ret < sizeof(atcmd_err_tbl)/sizeof(char *)) {
  //     Serial.printf("%s\r\n", atcmd_err_tbl[ret]);
  // } else {
  //     Serial.printf("%s\r\n", atcmd_err_tbl[1]);
  // }
  
}



void process_serial_input(uint8_t c)
{
  static char input[MAX_CMD_LEN + MAX_PARAM_LEN + 3];
  static int i = 0;

  /* overflow  */
  if (i >= MAX_CMD_LEN + MAX_PARAM_LEN + 2) //+2 /r/n
  {
        i = 0;
        Serial.printf("ERROR: Input buffer overflow\r\n");
        return;
  }

  if(c == '\r' or c == '\n')
  {
    input[i] = '\0';
    process_AT_Command(input);
    i=0;
  }
  else
  {
    input[i] = c;
    i++;
  }

  char ch = (char) c; // 将十六进制数转换为 char
  Serial.printf("Hex value: 0x%X, Character: %c\n", c, ch); // 打印结果

}



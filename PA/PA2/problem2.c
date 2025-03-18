#include <stdint.h>
#include <stdio.h>

int pc;
int registers[6];

enum {
  ADD,
  SUB,
  MUL,
  DIV,
  LET,
  PRINT
};

void
decode(uint32_t ins, uint32_t *ops)
{
  ops[0] = (ins & 57344) >> 13; //0b1110000000000000
  ops[1] = (ins & 7168) >> 10;  //0b0001110000000000
  ops[2] = (ins & 896) >> 7;   //0b0000001110000000
  ops[3] = (ins & 127);        //0b0000000001111111
}

void
exec(uint32_t *ops)
{
  uint32_t opcode = ops[0];
  uint32_t r1 = ops[1];
  uint32_t r2 = ops[2];
  uint32_t imm = ops[3];
  if (r1 == 0 && opcode != PRINT)
    return ;

  switch (opcode) {
    case ADD:
      registers[r1] += registers[r2];
      break;
    case SUB:
      registers[r1] -= registers[r2];
      break;
    case MUL:
      registers[r1] *= registers[r2];
      break;
    case DIV:
      registers[r1] /= registers[r2];
      break;
    case LET:
      registers[r1] = imm; 
      break;
    case PRINT:
      printf("x%d = %d\n", r1, registers[r1]);
      break;
    default:
      printf("Unknown opcode!\n");
  }
}

int 
main(void)
{
  int N;
  uint32_t ins;
  uint32_t ops[4] = {0};
  scanf("%d\n", &N);

  for (int i = 0; i < N; ++i)
  {
    scanf("%x", &ins);
    decode(ins, ops);
    exec(ops);
  }
  return 0;
}

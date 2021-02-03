typedef struct MyBitfield
{
    unsigned char data0 : 3;
    unsigned char data1 : 3;
    unsigned char data2 : 2;
    unsigned char data3 : 8;
} MyBitfield;

typedef struct MyDataType
{
    unsigned char data0;
    unsigned char data1;
    unsigned char data2;
    unsigned char data3;
} MyDataType;

typedef struct BitfieldExam {
    unsigned char Level : 3;
    unsigned char Direction : 1;
    unsigned char Attempts : 4;
    unsigned char BufferIndex;
    unsigned short DataSize;
} BitfieldExam;

int main(int argc, char* argv[]) {
    printf("MyBitfield size: %zd\n", sizeof(MyBitfield));
    printf("MyDataType size: %zd\n", sizeof(MyDataType));
    printf("BitfieldExam size: %zd\n", sizeof(BitfieldExam));
    return 0;
}

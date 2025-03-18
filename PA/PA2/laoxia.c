#include <stdio.h>
unsigned int cyclic_left_shift(unsigned int value, int shift);

int main(){
    int world_line , events;
    scanf("%d %d", &world_line, &events);

    unsigned int singularity_values[world_line];
    int edges[world_line];

    for (int i = 0; i < world_line; i++) {
        scanf("%u ", &singularity_values[i]);
        edges[i] = i; // 初始出边指向自身
    }

    //yunxing
    for (int i = 0; i < events; i++){
        int a,b;
        scanf("%d %d", &a, &b);
        a--; b--; // 0-based index

        if (a == b) {
            continue;
        }

        edges[a] = b; // a的出边指向b

        int current = a;//chushiuha
        unsigned int num = singularity_values[a];
        int shift = 1;
        int modified[1000] = {0}; // 标记每个世界线的奇点值是否被修改过
        while (1) {
            current = edges[current];
            if (current == a) { // 如果回到起始世界线，停止更新
                break;
            }
            if (current == a || modified[current]) { // 如果回到起始世界线或当前世界线已被修改，停止更新
                break;
            } 

            singularity_values[current] ^= cyclic_left_shift(num, shift);
            modified[current] = 1;
            shift++;

            // （防止无限循环）
            if (shift > 3 * world_line) {
                break;
            }
        }
   
    }

    //zhaozuixiao
    unsigned int value = singularity_values[0];
    int index = 0;
    for (int i = 0; i < world_line; i++) {
        if (singularity_values[i] < value) {
            value = singularity_values[i];
            index = i;
        }}

    printf("%u\n%d\n",index + 1,value);
}



unsigned int cyclic_left_shift(unsigned int value, int shift) {
    return ((value << shift) | (value >> (32 - shift)));
}

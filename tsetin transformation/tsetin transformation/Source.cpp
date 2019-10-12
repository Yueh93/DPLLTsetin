#include <iostream>
#include <stdlib.h>
#include <math.h>

int validity = 0;

int DPLL(int n, int maxline, int **tsetin,int maxnum, int *match ,int *output ) {

	//正的作用
	for (int i = 0; i < maxline; i++) {
		for (int j = 0; j < 14; j++) {
			if (tsetin[i][0] == n + 48 && j == 0) {
				match[i] += 1;
			}else if (tsetin[i][j] != '-' && tsetin[i][j + 1] == n +48) {
				match[i] += 1;
			}
		}
	}
	if (n < maxnum) {
		int next = 0;
		while (output[next] != 0) {
			next++;
		}
		output[next] = ' ';
		output[next+1] = 48+n;
		output[next+2] = ',';
	}

	if (n == maxnum  ) {
		int count = 0;
		for (int i = 0; i < maxline; i++) {
			if (match[i] == 0)
				count++;
		}
		if (count == 0) {
			int next = 0;
			while (output[next] != 0) {
				printf("%c", output[next]);
				next++;
			}
			printf("%d satisfied\n", n);
		}
		else {
			validity++;
		}
		
	}
	if (n < maxnum) {
		DPLL(n+1, maxline, tsetin, maxnum, match, output );
	}
	//消除正的作用
	for (int i = 0; i < maxline; i++) {
		for (int j = 0; j < 14; j++) {
			if (tsetin[i][0] == n + 48 && j == 0) {
				match[i] -= 1;
			}
			else if (tsetin[i][j] != '-' && tsetin[i][j + 1] == n + 48) {
				match[i] -= 1;
			}
		}
	}
	if (n < maxnum) {
		int next = 0;
		while (output[next] != 48+n) {
			next++;
		}
		output[next-1] = 0;
		output[next] = 0;
		output[next+1] = 0;
	}
	//負的作用
	for (int i = 0; i < maxline; i++) {
		for (int j = 0; j < 14; j++) {
			if (tsetin[i][j] == '-' && tsetin[i][j + 1] == n +48) {
				match[i] += 1;
			}
		}
	}
	if (n < maxnum) {
		int next = 0;
		while (output[next] != 0) {
			next++;
		}
		output[next] = '-';
		output[next+1] = 48+n;
		output[next+2] = ',';
	}
	if (n == maxnum  ) {
		int count = 0;
		for (int i = 0; i < maxline; i++) {
			if (match[i] == 0)
				count++;
		}
		if (count == 0) {
			int next = 0;
			while (output[next] != 0) {
				printf("%c", output[next]);
				next++;
			}
			printf("-%d satisfied\n", n);
		}
		else
			validity++;
	}
	if (n < maxnum) {
		DPLL(n + 1, maxline, tsetin, maxnum, match, output );
	}
	//消除負的作用
	for (int i = 0; i < maxline; i++) {
		for (int j = 0; j < 14; j++) {
			if (tsetin[i][j] == '-' && tsetin[i][j + 1] == n +48) {
				match[i] -= 1;
			}
		}
	}
	if (n < maxnum) {
		int next = 0;
		while (output[next] != 48+n) {
			next++;
		}
		output[next-1] = 0;
		output[next] = 0;
		output[next+1] = 0;
	}

	return 0;

}

int power(int n) {
	int a = 1;
	if (n > 0)
		a = 2 * power(n - 1);
	return a;
}

int main() {

	

	int get_clause[15][15] = { 0 }; //紀錄clause的數目和元素
	int stream[50] = { 0 };//儲存輸入的formula
	int i_left_par = 0, i_right_par = 0;//紀錄左右括號數
	int left_par[15] = { 0 }, right_par[15] = { 0 };//紀錄每個對應的括號
	int signed_right[15] = { 0 }; //查看是否已經有對應
	int i_stream = 0, count = 0;//count為right_par找值
	int placeofrightpar[15] = { 0 }; //右括號位置
	int placeofimp[5] = { 0 };//implies的位置
	int imply=0;
	int placeofequ = 0;//place of the symbol logical equivalance
	int equivalence = 0;
	int expand[15] = { 0 };//擴展元素用
	int middle = 0; //最外層的符號

	// 第一部分得到左括號、右括號、implies、ligical equvalence符號的位置
	while ((stream[i_stream] = getchar()) != '#') {
		
		if (stream[i_stream] == '(') {
			i_left_par ++;
			left_par[i_left_par-1] = i_left_par;
		}else if (stream[i_stream] == ')') {
			i_right_par ++;
			if( signed_right[i_left_par] == 0){
				right_par[i_right_par-1] =i_left_par;
				signed_right[i_left_par] = 1;
				placeofrightpar[i_right_par - 1] = i_stream+1;
			}else{
				while(signed_right[i_left_par - count] != 0 || i_left_par == count){
				count++;
				}
				signed_right[i_left_par - count] = 1;
				right_par[i_right_par-1] =i_left_par - count;
				placeofrightpar[i_right_par - 1] = i_stream+1;
				count = 0;
			}
		}else if (stream[i_stream] == '>' && i_left_par != i_right_par) {//紀錄impliy的位置
			placeofimp[imply] = i_stream;
			imply++;
		}
		else if (stream[i_stream] == '>' && i_left_par == i_right_par) {
			middle = i_stream;
		}else if (stream[i_stream] == '~') {
			placeofequ = i_stream;
		}

		i_stream++;
	}
	//第一部分結束


	//第二部分得到每一對括號中的值
	i_stream = 0; count = 0; //歸零讓下一個函數利用 , i_stream供循環使用. count尋找下一個clause空間
	int numberofclause = 0, placeofleftpar = 0; //placeofleftpar是讀取開始的數字
	
	while (right_par[i_stream] != 0) { 
		
		while (numberofclause != right_par[i_stream]) { //第N個左括號數位置
			if (stream[placeofleftpar] == '(') {
				numberofclause++;
			}
			placeofleftpar++;
		}

		numberofclause = 0; //歸零改計算左右括號的次數
		while (placeofleftpar != placeofrightpar[i_stream]) {
			if (stream[placeofleftpar] == '(') {
				numberofclause ++;  //暫時借用來當閥門
			}else if ( stream[placeofleftpar] == ')') {
				numberofclause --;
			}else if ((stream[placeofleftpar] >= 48 && stream[placeofleftpar] <= 57 && numberofclause == 0)
				||(stream[placeofleftpar] == 45 && numberofclause == 0)  ) {
				while (get_clause[right_par[i_stream]][count] != 0) {//找到下一個數字位置
					count++;
				}
				get_clause[right_par[i_stream]][count] = stream[placeofleftpar];
				count = 0;
			}
			
			placeofleftpar++;
		}

		placeofleftpar = 0;
		numberofclause = 0;
		i_stream++;
	}
	//第二部分結束



	int findbiggest = 48;  //找到最大的數字並用來命名新的clauses
	for (int i = 0; i < 50; i++) {
		if (stream[i] >= 48 && stream[i] <= 57 && stream[i] > findbiggest)
			findbiggest = stream[i];
	}
	findbiggest++;


	//第三部分將imply對應的clause重新改寫
	i_stream = 0; count = 0; imply = 0; equivalence = 0;//歸零待利用 count是左括號數 i_stream循環數
	int remember[5] = { 0 };
	int re = 0;
	while (placeofimp[i_stream] != 0) {

		for (int i = 0; i < placeofimp[i_stream]; i++) {//計算到imply前的左括號數
			if (stream[i] == '(')
				count++;
			if (stream[i] == ')')
				equivalence++;
		}
		if (count != equivalence ) {
			while (get_clause[count - 1][imply] != 0) {
				imply++;
			}
			

			if (get_clause[count +1 ][0] == 0 && get_clause[count + 1][1] == 0) {//如果最後一組為空組(表示目前在外面imply)則紀錄給remember留到第四部賦值
				remember[re] = count;
				re++;
			}else if (get_clause[count][1] == 0 && get_clause[count][0] != '-' && get_clause[count][0] != 0) {//如果第一組不為零
				get_clause[count - 1][imply] = '-';
				get_clause[count - 1][imply + 1] = get_clause[count][0];
				imply += 2;
				for (int i = 0; i < 15; i++)
					get_clause[count][i] = 0;
			}
			else if (get_clause[count][0] == '-' && get_clause[count][2] == 0) {//如果第一組為-
				get_clause[count - 1][imply] = get_clause[count][1];
				imply++;
				for (int i = 0; i < 15; i++)
					get_clause[count][i] = 0;
			}else {//如果第一組為許多元素的組合則更改命名並加-
				get_clause[count - 1][imply] = '-';
				get_clause[count - 1][imply + 1] = findbiggest;
				imply += 2;
				printf("Introduce a new variable for the subformula: \n");
				printf("%c~",findbiggest);
				for (int i = 0; i < 15; i++)
					printf("%c",get_clause[count][i]);
				printf("\n");
				get_clause[count + 2][0] = '-';
				get_clause[count + 2][1] = findbiggest;
				get_clause[count + 3][0] = findbiggest;
				get_clause[count + 4][0] = findbiggest;
				if (get_clause[count][0] == '-' && get_clause[count][2] == '-') {
					get_clause[count + 2][2] = '-';
					get_clause[count + 2][3] = get_clause[count][1];
					get_clause[count + 2][4] = '-';
					get_clause[count + 2][5] = get_clause[count][3];
					get_clause[count + 3][1] = get_clause[count][1];
					get_clause[count + 4][1] = get_clause[count][3];
				}else if (get_clause[count][0] == '-' && get_clause[count][2] != '-') {
					get_clause[count + 2][2] = '-';
					get_clause[count + 2][3] = get_clause[count][1];
					get_clause[count + 2][4] = get_clause[count][2];
					get_clause[count + 3][1] = get_clause[count][1];
					get_clause[count + 4][1] = '-';
					get_clause[count + 4][2] = get_clause[count][2];
				}else if (get_clause[count][0] != '-' && get_clause[count][1] == '-') {
					get_clause[count + 2][2] = get_clause[count][0];
					get_clause[count + 2][3] = '-';
					get_clause[count + 2][4] = get_clause[count][2];
					get_clause[count + 3][1] = '-';
					get_clause[count + 3][2] = get_clause[count][0];
					get_clause[count + 4][1] = get_clause[count][2];
				}else if (get_clause[count][0] != '-' && get_clause[count][1] != '-') {
					get_clause[count + 2][2] = get_clause[count][0];
					get_clause[count + 2][3] = get_clause[count][1];
					get_clause[count + 3][1] = '-';
					get_clause[count + 3][2] = get_clause[count][0];
					get_clause[count + 4][1] = '-';
					get_clause[count + 4][2] = get_clause[count][2];
				}
			}
			if (get_clause[count + 1][1] == 0 ) {//加第一個元素，如果是空組則加零到最後一個元素(不變)
				get_clause[count - 1][imply] = get_clause[count + 1][0];
				for (int i = 0; i < 15; i++)
					get_clause[count + 1][i] = 0;
			}else if(get_clause[count + 1][0] == '-' && get_clause[count + 1][2] == 0){
				get_clause[count - 1][imply] = '-';
				get_clause[count - 1][imply+1] = get_clause[count + 1][1];
				for (int i = 0; i < 15; i++)
					get_clause[count+1][i] = 0;
			}else {
				get_clause[count - 1][imply] = findbiggest;
				findbiggest++;
			}
			i_stream++;
			imply = 0;
		}
		count = 0;
		equivalence = 0;
	}
	//第三部分結束

	//第四部份是第三部分的延續
	for (int i = 0; i < 4; i++) {
		if (remember[i] != 0) {
			while (get_clause[remember[i] - 1][imply] != 0) {
				imply++;
			}
			
			if (get_clause[remember[i]][1] == 0) {
				get_clause[remember[i] - 1][imply] = '-';
				get_clause[remember[i] - 1][imply + 1] = get_clause[remember[i]][0];
				imply += 2;
			}
			else if (get_clause[remember[i]][0] == '-' && get_clause[remember[i]][2] == 0) {
				get_clause[remember[i] - 1][imply] = get_clause[remember[i]][1];
				imply++;
			}
			else {
				get_clause[remember[i] - 1][imply] = '-';
				get_clause[remember[i] - 1][imply + 1] = findbiggest;
				imply += 2;
				findbiggest++;
			}
			if (get_clause[remember[i] + 1][1] == 0) {
				get_clause[remember[i] - 1][imply] = get_clause[remember[i] + 1][0];
			}
			else if (get_clause[remember[i] + 1][0] == '-' && get_clause[remember[i] + 1][2] == 0) {
				get_clause[remember[i] - 1][imply] = '-';
				get_clause[remember[i] - 1][imply + 1] = get_clause[remember[i] + 1][1];
			}
			else {
				get_clause[remember[i] - 1][imply] = findbiggest;

				for (int j = 0; j < 15; j++) {
					expand[j] = get_clause[remember[i] + 1][j];
					get_clause[remember[i] + 1][j] = 0;
					get_clause[remember[i]][j] = 0;
				}
				
				get_clause[remember[i] + 1][0] = findbiggest;
				get_clause[remember[i] + 1][1] = '~';
				for (int j = 0; j < 13; j++) {
					get_clause[remember[i] + 1][j + 2] = expand[j];
					expand[j] = 0;
				}
				findbiggest++;
			}
		}
		imply = 0;
	}
	//第四部份結束
	
	i_stream = 0; count = 0; equivalence = 0; imply = 0;
	
	//第五部分:如果中間是imply
	if(middle != 0){
		for (int i = 0; i < middle; i++)
			if (stream[i] == '(')
				count++;
		if (get_clause[1][0] == '-') {
			get_clause[i_left_par + 1][0] = get_clause[1][1];
		}else {
			get_clause[i_left_par + 1][0] = '-';
			get_clause[i_left_par + 1][1] = get_clause[1][0];
		}
		if (get_clause[1][1] == '-' ) {
			get_clause[i_left_par + 2][0] = get_clause[1][2];
		}else if (get_clause[1][2] != '-' && get_clause[1][0] == '-') {
			get_clause[i_left_par + 2][0] = '-';
			get_clause[i_left_par + 2][1] = get_clause[1][2];
		}else if (get_clause[1][2] == '-' && get_clause[1][0] == '-') {
			get_clause[i_left_par + 2][0] = get_clause[1][3];
		}else if (get_clause[1][1] != '-' && get_clause[1][0] != '-') {
			get_clause[i_left_par + 2][0] = '-';
			get_clause[i_left_par + 2][1] = get_clause[1][1];
		}
		while (get_clause[i_left_par + 1][imply] != 0) {
			imply++;
		}
		for (int i = 0; i < 15 - imply; i++)
			get_clause[i_left_par + 1][imply + i] = get_clause[count + 1][i];
		imply = 0;
		while (get_clause[i_left_par + 2][imply] != 0) {
			imply++;
		}
		for (int i = 0; i < 15 - imply; i++)
			get_clause[i_left_par + 2][imply + i] = get_clause[count + 1][i];
		imply = 0;
	}
	//第五部分結束

	count = 0; equivalence = 0; imply = 0;
	//第六部分:如果中間是等號
	if (placeofequ != 0) {
		for (int i = 0; i < placeofequ; i++)
			if (stream[i] == '(')
				count++;
		//第一組
		if (get_clause[1][0] == '-') {
			get_clause[i_left_par + 1][0] = get_clause[1][1];
		}
		else {
			get_clause[i_left_par + 1][0] = '-';
			get_clause[i_left_par + 1][1] = get_clause[1][0];
		}
		//第二組
		if (get_clause[1][1] == '-') {
			get_clause[i_left_par + 2][0] = get_clause[1][2];
		}
		else if (get_clause[1][2] != '-' && get_clause[1][0] == '-') {
			get_clause[i_left_par + 2][0] = '-';
			get_clause[i_left_par + 2][1] = get_clause[1][2];
		}
		else if (get_clause[1][2] == '-' && get_clause[1][0] == '-') {
			get_clause[i_left_par + 2][0] = get_clause[1][3];
		}
		else if (get_clause[1][1] != '-' && get_clause[1][0] != '-') {
			get_clause[i_left_par + 2][0] = '-';
			get_clause[i_left_par + 2][1] = get_clause[1][1];
		}
		//右側第一組
		if (get_clause[count+1][0] == '-') {
			get_clause[i_left_par + 3][0] = get_clause[count+1][1];
		}
		else {
			get_clause[i_left_par + 3][0] = '-';
			get_clause[i_left_par + 3][1] = get_clause[count+1][0];
		}
		//右側第二組
		if (get_clause[count+1][1] == '-') {
			get_clause[i_left_par + 4][0] = get_clause[count + 1][2];
		}
		else if (get_clause[count + 1][2] != '-' && get_clause[count + 1][0] == '-') {
			get_clause[i_left_par + 4][0] = '-';
			get_clause[i_left_par + 4][1] = get_clause[count + 1][2];
		}
		else if (get_clause[count + 1][2] == '-' && get_clause[count + 1][0] == '-') {
			get_clause[i_left_par + 4][0] = get_clause[count + 1][3];
		}
		else if (get_clause[count + 1][1] != '-' && get_clause[count + 1][0] != '-') {
			get_clause[i_left_par + 4][0] = '-';
			get_clause[i_left_par + 4][1] = get_clause[count + 1][1];
		}
		
		//補足
		while (get_clause[i_left_par + 1][imply] != 0) {
			imply++;
		}
		for (int i = 0; i < 15 - imply; i++)
			get_clause[i_left_par + 1][imply + i] = get_clause[count + 1][i];
		imply = 0;
		while (get_clause[i_left_par + 2][imply] != 0) {
			imply++;
		}
		for (int i = 0; i < 15 - imply; i++)
			get_clause[i_left_par + 2][imply + i] = get_clause[count + 1][i];
		imply = 0;
		while (get_clause[i_left_par + 3][imply] != 0) {
			imply++;
		}
		for (int i = 0; i < 15 - imply; i++)
			get_clause[i_left_par + 3][imply + i] = get_clause[1][i];
		imply = 0;
		while (get_clause[i_left_par + 4][imply] != 0) {
			imply++;
		}
		for (int i = 0; i < 15 - imply; i++)
			get_clause[i_left_par + 4][imply + i] = get_clause[1][i];
		imply = 0;
	}
	//第六部分結束
	
	
	//第七部分 :剩下的等號重寫
	int get= i_left_par, stop = 0;
	for (int i = 0; i < 15; i++) {
		if (get_clause[i][1] == '~') {
			printf("Introduce a new variables for the formula: \n");
			if (stop == 0) {
				get = i - 1;
				stop = 1;
			}
			for (int j = 0; j < 15; j++)
				printf("%c", get_clause[i][j]);
			printf("\n");
			for (int j = 0; j < 15; j++) {
				expand[j] = get_clause[i][j];
				get_clause[i][j] = 0;
			}
			get_clause[i][0] = '-';
			get_clause[i][1] = expand[0];
			
			for (int j = 0; j < 13; j++) {
				get_clause[i][j+2] = expand[j+2];
			}
			get_clause[i + 1][0] = expand[0];
			if (expand[2] == '-') {
				get_clause[i + 1][1] = expand[3];
			}
			else {
				get_clause[i + 1][1] = '-';
				get_clause[i + 1][2] = expand[2];
			}
			get_clause[i + 2][0] = expand[0];
			if (expand[3] == '-') {
				get_clause[i + 2][1] = expand[4];
			}
			else if (expand[4] != '-' && expand[2] == '-') {
				get_clause[i + 2][1] = '-';
				get_clause[i + 2][2] = expand[4];
			}
			else if (expand[4] == '-' && expand[2] == '-') {
				get_clause[i + 2][1] = expand[5];
			}
			else if (expand[3] != '-' && expand[2] != '-') {
				get_clause[i + 2][1] = '-';
				get_clause[i + 2][2] = expand[3];
			}

		}
	}
		
	for (int i = 0; i < get; i++)
		for (int j = 0; j < 15; j++)
			get_clause[i][j] = 0;
			

	//第七部分結束
	count = 0;
	//總結
	for (int i = 0; i < 15; i++) {
		if (get_clause[i][0] != 0)
			count++;
	}
	int ** tsetin = (int**) malloc( count * sizeof( int*));
	for (int i = 0; i < count; i++){
		tsetin[i] = (int*)malloc(15 * sizeof(int*));
	}
	int* change = (int*)malloc(count * sizeof(int));
	for (int i = 0; i < count; i++) {
		change[i] = 0;
	}
	equivalence = 0;
	for (int i = 0; i < 15; i++) {
		if (get_clause[i][0] != 0) {
			for (int j = 0; j < 15; j++) {
				tsetin[equivalence][j] = get_clause[i][j];
				//printf("%c", tsetin[equivalence][j]);
			}
			//printf("\n");
			equivalence++;
		}
	}


	int output[20] = { 0 };
	findbiggest--;
	
	DPLL(1, count, tsetin, findbiggest -48, change, output );


	if (validity == 0) {
		printf("This fomula is valid\n");
	}
	else if (validity == power(findbiggest - 48)) {
		printf("This fomula is unsatisfiable\n");
	}
	else if (validity > 0) {
		printf("This fomula is satisfiabled\n");
	}
	
	system("pause");

	return 0;
}
#include<cstdio>

int main()
{
	int times=1;
	while(1){
		bool empty=true;
		char board[12][12];
		bool white=false;
		bool black=false;
		for(int r=0; r<8; r++)
			scanf("%s", board[r]);
		for(int r=0; r<8; r++){
			for(int c=0; c<8; c++){
				if(board[r][c] != '.'){
					empty = false;
					if(board[r][c] == 'p'){
						if((r+1)<8 && ( ((c-1)>=0 && board[r+1][c-1]=='K') || ((c+1)<8 && board[r+1][c+1]=='K') ) )
							white = true;
					}
					if(board[r][c] == 'P'){
						if((r-1)>=0 && ( ((c-1)>=0 && board[r-1][c-1]=='k') || ((c+1)<8 && board[r-1][c+1]=='k') ) )
							black = true;
					}
					if(board[r][c] == 'r' || board[r][c] == 'q'){
						for(int tr=r+1; board[tr][c]=='.'||board[tr][c]=='K'; tr++){
							if(board[tr][c] == 'K'){
								white = true;
								break;
							}
						}
						for(int tr=r-1; tr>=0&&(board[tr][c]=='.'||board[tr][c]=='K'); tr--){
							if(board[tr][c] == 'K'){
								white = true;
								break;
							}
						}
						for(int tc=c+1; board[r][tc]=='.'||board[r][tc]=='K'; tc++){
							if(board[r][tc] == 'K'){
								white = true;
								break;
							}
						}
						for(int tc=c-1; tc>=0&&(board[r][tc]=='.'||board[r][tc]=='K'); tc--){
							if(board[r][tc] == 'K'){
								white = true;
								break;
							}
						}
					}
					if(board[r][c] == 'R' || board[r][c] == 'Q'){
						for(int tr=r+1; board[tr][c]=='.'||board[tr][c]=='k'; tr++){
							if(board[tr][c] == 'k')
								black = true;
						}
						for(int tr=r-1; tr>=0&&(board[tr][c]=='.'||board[tr][c]=='k'); tr--){
							if(board[tr][c] == 'k')
								black = true;
						}
						for(int tc=c+1; board[r][tc]=='.'||board[r][tc]=='k'; tc++){
							if(board[r][tc] == 'k')
								black = true;
						}
						for(int tc=c-1; tc>=0&&(board[r][tc]=='.'||board[r][tc]=='k'); tc--){
							if(board[r][tc] == 'k')
								black = true;
						}
					}
					if(board[r][c] == 'b' || board[r][c] == 'q'){
						for(int tr=r+1,tc=c+1; board[tr][tc]=='.'||board[tr][tc]=='K'; tr++, tc++){
							if(board[tr][tc] == 'K'){
								white = true;
								break;
							}
						}
						for(int tr=r+1,tc=c-1; tc>=0&&(board[tr][tc]=='.'||board[tr][tc]=='K'); tr++, tc--){
							if(board[tr][tc] == 'K'){
								white = true;
								break;
							}
						}
						for(int tr=r-1,tc=c+1; tr>=0&&(board[tr][tc]=='.'||board[tr][tc]=='K'); tr--, tc++){
							if(board[tr][tc] == 'K'){
								white = true;
								break;
							}
						}
						for(int tr=r-1,tc=c-1; tr>=0&&tc>=0&&(board[tr][tc]=='.'||board[tr][tc]=='K'); tr--, tc--){
							if(board[tr][tc] == 'K'){
								white = true;
								break;
							}
						}
					}
					if(board[r][c] == 'B' || board[r][c] == 'Q'){
						for(int tr=r+1,tc=c+1; board[tr][tc]=='.'||board[tr][tc]=='k'; tr++, tc++){
							if(board[tr][tc] == 'k')
								black = true;
						}
						for(int tr=r+1,tc=c-1; tc>=0&&(board[tr][tc]=='.'||board[tr][tc]=='k'); tr++, tc--){
							if(board[tr][tc] == 'k')
								black = true;
						}
						for(int tr=r-1,tc=c+1; tr>=0&&(board[tr][tc]=='.'||board[tr][tc]=='k'); tr--, tc++){
							if(board[tr][tc] == 'k')
								black = true;
						}
						for(int tr=r-1,tc=c-1; tr>=0&&tc>=0&&(board[tr][tc]=='.'||board[tr][tc]=='k'); tr--, tc--){
							if(board[tr][tc] == 'k')
								black = true;
						}
					}
					if(board[r][c] == 'n'){
						const int dir[8][2] = { {1,2}, {1,-2}, {-1,2}, {-1,-2}, {2,1}, {2,-1}, {-2,1}, {-2,-1}};
						for(int i=0; i<8; i++){
							int tr=r+dir[i][0];
							int tc=c+dir[i][1];
							if(tr >= 0 && tr < 8 && tc >= 0 && tc < 8 && board[tr][tc]=='K')
								white = true;
						}
					}
					if(board[r][c] == 'N'){
						const int dir[8][2] = { {1,2}, {1,-2}, {-1,2}, {-1,-2}, {2,1}, {2,-1}, {-2,1}, {-2,-1}};
						for(int i=0; i<8; i++){
							int tr=r+dir[i][0];
							int tc=c+dir[i][1];
							if(tr >= 0 && tr < 8 && tc >= 0 && tc < 8 && board[tr][tc]=='k')
								black = true;
						}
					}
				}
			}
		}
		if(empty)
			break;
		if(black)
			printf("Game #%d: black king is in check.\n", times);
		else if(white)
			printf("Game #%d: white king is in check.\n", times);
		else
			printf("Game #%d: no king is in check.\n", times);
		times++;
	}
	return 0;
}

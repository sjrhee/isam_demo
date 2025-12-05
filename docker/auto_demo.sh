#!/bin/bash
# C-ISAM 자동 데모 실행 스크립트
# 각 단계별로 프로그램을 실행하고 결과를 보여줍니다.

BIN="/app/demo/build"
DATA="/app/data"

# 텍스트 색상
GREEN='\033[0;32m'
NC='\033[0m'

echo -e "${GREEN}=== C-ISAM 데모 자동 실행 시작 ===${NC}"
echo "데이터 디렉토리: $DATA"
cd $DATA

# 1. 파일 시스템 초기화
echo -e "\n${GREEN}[1/7] 파일 시스템 생성 (bld_file)${NC}"
echo "----------------------------------------"
# 기존 파일 삭제
rm -f *.dat *.idx
$BIN/bld_file
ls -l *.dat *.idx

# 2. 인덱스 추가
echo -e "\n${GREEN}[2/7] 보조 인덱스 추가 (add_indx)${NC}"
echo "----------------------------------------"
$BIN/add_indx
ls -l *.idx

# 3. 레코드 추가 (자동 입력)
echo -e "\n${GREEN}[3/7] 레코드 추가 (add_rcrd)${NC}"
echo "----------------------------------------"
# 입력 데이터 순서:
# 1. Emp No (0 to exit)
# 2. Last Name
# 3. First Name
# 4. Address
# 5. City
# --- getperform() ---
# 6. Start Date
# 7. Starting Salary
# 8. Title

# 1001, Kim, Minsoo, 123 Main St, Seoul, 01/01/24, 50000, Developer
# 마지막에 0을 입력하여 프로그램 종료
printf "1001\nKim\nMinsoo\n123 Main St\nSeoul\n01/01/24\n50000\nDeveloper\n0\n" | $BIN/add_rcrd
echo "-> 레코드 1 추가 완료"

# 1002, Lee, Jieun, 456 Park Ave, Busan, 02/01/24, 60000, Designer
printf "1002\nLee\nJieun\n456 Park Ave\nBusan\n02/01/24\n60000\nDesigner\n0\n" | $BIN/add_rcrd
echo "-> 레코드 2 추가 완료"

# 4. 데이터 조회
echo -e "\n${GREEN}[4/7] 데이터 조회 (sqntl_rd)${NC}"
echo "----------------------------------------"
$BIN/sqntl_rd

# 5. 성과 기록 추가
echo -e "\n${GREEN}[5/7] 성과 기록 추가 (chaining)${NC}"
echo "----------------------------------------"
# 입력 데이터 순서:
# 1. Emp No
# 2. Review Date
# 3. Rating (p/f/g)
# 4. Title After Review

# 사번 1001, 06/01/24, good, Senior Dev
# 마지막에 0을 입력하여 프로그램 종료
printf "1001\n06/01/24\ng\nSenior Dev\n0\n" | $BIN/chaining
echo "-> 사번 1001 성과 기록 추가 완료"

# 6. 업데이트 후 조회
echo -e "\n${GREEN}[6/7] 업데이트 결과 확인 (sqntl_rd)${NC}"
echo "----------------------------------------"
$BIN/sqntl_rd

echo -e "\n${GREEN}=== 데모 종료 ===${NC}"
echo "생성된 데이터 파일은 호스트의 docker/data 폴더에 저장되었습니다."

#!/bin/bash
# Docker 실행 스크립트
# 호스트의 데이터 폴더와 자동 실행 스크립트를 마운트하여 실행합니다.

# 스크립트의 절대 경로 확인
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# 데이터 디렉토리 생성
mkdir -p "$SCRIPT_DIR/data"

echo "Running C-ISAM Demo..."
echo "Data directory: $SCRIPT_DIR/data"
echo ""

# 컨테이너 실행
# -v $SCRIPT_DIR/data:/app/data : 데이터 영속성 및 공유
# -v $SCRIPT_DIR/auto_demo.sh:/app/auto_demo.sh : 스크립트 수정 편의성
docker run -it --rm \
  -v "$SCRIPT_DIR/data":/app/data \
  -v "$SCRIPT_DIR/auto_demo.sh":/app/auto_demo.sh \
  cisam-demo \
  /bin/bash /app/auto_demo.sh

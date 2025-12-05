#!/bin/bash
# Docker 이미지 빌드 스크립트
# 개발자가 수정하기 쉽도록 단순화됨

# 스크립트의 절대 경로 확인
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

echo "Building Docker image..."
docker build -f "$SCRIPT_DIR/Dockerfile" -t cisam-demo "$PROJECT_ROOT"

# C-ISAM 데모

## 📋 개요

C-ISAM (Indexed Sequential Access Method)을 사용하는 직원 관리 시스템 데모 프로젝트입니다.  
IBM의 인덱스 순차 접근 방식을 활용하여 직원 데이터와 성과 기록을 관리합니다.

---

## 🚀 빠른 시작

### 방법 1: Docker 사용 (권장)

Docker를 사용하면 환경 설정 없이 바로 실행할 수 있습니다.

```bash
# 1. Docker 이미지 빌드
./docker/build.sh

# 2. 프로그램 실행
./docker/run.sh bld_file    # 파일 시스템 생성
./docker/run.sh add_indx    # 인덱스 추가
./docker/run.sh add_rcrd    # 레코드 추가
./docker/run.sh sqntl_rd    # 데이터 조회

# 3. 대화형 셸
./docker/run.sh shell
```

**상세 가이드**: [docker/README.md](docker/README.md)  
**빠른 참조**: [docker/QUICKSTART.md](docker/QUICKSTART.md)

### 방법 2: 로컬 컴파일

```bash
# 컴파일
make

# 실행
./demo/build/bld_file
./demo/build/add_indx
./demo/build/add_rcrd
./demo/build/sqntl_rd
```

---

## 📁 프로젝트 구조

```
CISAM/
├── demo/                         # 데모 프로젝트
│   ├── src/                      # 소스 코드
│   │   ├── bld_file.c           # 파일 시스템 초기화
│   │   ├── add_indx.c           # 인덱스 추가
│   │   ├── add_rcrd.c           # 레코드 추가
│   │   ├── upd_file.c           # 레코드 수정/삭제
│   │   ├── sqntl_rd.c           # 순차 읽기
│   │   ├── chaining.c           # 성과 기록 추가
│   │   └── transctn.c           # 트랜잭션 작업
│   └── build/                    # 컴파일된 실행파일
├── docker/                       # Docker 관련 파일
│   ├── Dockerfile               # Docker 이미지 정의
│   ├── build.sh                 # 이미지 빌드 스크립트
│   ├── run.sh                   # 컨테이너 실행 스크립트
│   ├── docker-compose.yml       # Docker Compose 설정
│   ├── README.md                # Docker 상세 가이드
│   └── QUICKSTART.md            # Docker 빠른 참조
├── include/                      # 헤더 파일
│   ├── isam.h                   # C-ISAM 헤더
│   └── decimal.h                # Decimal 타입 헤더
├── lib/                          # 라이브러리
│   └── llib-lisam               # C-ISAM 라이브러리
├── Makefile                      # 빌드 설정
├── SOURCE_DOCUMENTATION.md       # 소스 코드 상세 문서
└── README.md                     # 이 파일
```

---

## 📚 프로그램 설명

### 실행 순서

```
1. bld_file    → 파일 시스템 생성
2. add_indx    → 검색 인덱스 추가
3. add_rcrd    → 첫 직원 데이터 입력
4. sqntl_rd    → 입력된 데이터 확인 (필요시)
5. chaining    → 성과 정보 추가
6. upd_file    → 데이터 수정/삭제
7. transctn    → 트랜잭션 기반 작업
```

### 각 프로그램 기능

| 프로그램 | 기능 | 설명 |
|---------|------|------|
| `bld_file` | 파일 시스템 빌드 | 직원/성과 파일 생성 및 기본 인덱스 설정 |
| `add_indx` | 2차 인덱스 추가 | 성(Last Name), 급여 필드에 보조 인덱스 생성 |
| `add_rcrd` | 레코드 추가 | 새로운 직원 및 성과 기록 입력 |
| `upd_file` | 레코드 수정/삭제 | 직원 정보 수정 및 삭제 (참조 무결성 유지) |
| `sqntl_rd` | 순차 읽기 | 직원 번호 순으로 전체 직원 목록 조회 |
| `chaining` | 성과 기록 추가 | 직원의 성과 정보 입력 및 급여 인상 계산 |
| `transctn` | 트랜잭션 작업 | 로깅 및 복구 기능을 포함한 안정적인 데이터 추가 |

---

## 🛠️ 컴파일 및 실행

### Make 명령어

```bash
make              # 모든 파일 컴파일
make clean        # 빌드 결과 삭제
make clean-obj    # 오브젝트 파일만 삭제
make rebuild      # 전체 재빌드
make help         # 도움말 표시
```

### 프로그램 실행

```bash
./demo/build/bld_file        # 파일 시스템 생성
./demo/build/add_indx        # 인덱스 추가
./demo/build/add_rcrd        # 레코드 추가
./demo/build/upd_file        # 레코드 수정/삭제
./demo/build/sqntl_rd        # 데이터 조회
./demo/build/chaining        # 성과 기록 추가
./demo/build/transctn        # 트랜잭션 작업
```

---

## 🐳 Docker 사용법

### 기본 사용

```bash
# 이미지 빌드
./docker/build.sh

# 프로그램 실행
./docker/run.sh <program_name>

# 예제
./docker/run.sh bld_file
./docker/run.sh add_rcrd
./docker/run.sh sqntl_rd
```

### Docker Compose

```bash
# 컨테이너 시작
cd docker
docker-compose up -d

# 컨테이너 접속
docker-compose exec cisam-demo /bin/bash

# 컨테이너 중지
docker-compose down
```

### 상세 가이드

- **Docker 상세 가이드**: [docker/README.md](docker/README.md)
- **빠른 참조**: [docker/QUICKSTART.md](docker/QUICKSTART.md)

---

## 📖 문서

- **[SOURCE_DOCUMENTATION.md](SOURCE_DOCUMENTATION.md)**: 소스 코드 상세 설명
  - 각 프로그램의 목적과 기능
  - 데이터 구조
  - ISAM 함수 레퍼런스
  - 사용 예제

- **[docker/README.md](docker/README.md)**: Docker 사용 가이드
  - Docker 설치 및 설정
  - 이미지 빌드 및 실행
  - 데이터 영속성 관리
  - 문제 해결

- **[docker/QUICKSTART.md](docker/QUICKSTART.md)**: Docker 빠른 참조
  - 자주 사용하는 명령어
  - 일반적인 워크플로우
  - 치트시트

---

## 🔧 시스템 요구사항

### 로컬 컴파일

- **OS**: Linux (Ubuntu/Debian 권장)
- **컴파일러**: GCC
- **라이브러리**: C-ISAM 라이브러리 (포함됨)
- **Make**: GNU Make

### Docker 사용

- **Docker**: 20.10 이상
- **Docker Compose**: 1.29 이상 (선택사항)
- **OS**: Linux, macOS, Windows (WSL2)

---

## 📊 데이터 구조

### 직원(Employee) 파일
- **레코드 크기**: 84바이트
- **주요 필드**:
  - 직원 번호: 4바이트 (long 타입)
  - 성(Last Name): 20바이트
  - 기타 정보: 60바이트

### 성과(Performance) 파일
- **레코드 크기**: 50바이트
- **주요 필드**:
  - 직원 번호: 4바이트
  - 성과 등급: 6바이트
  - 급여: 포함

---

## 🎯 사용 예제

### 시나리오 1: 초기 설정 및 데이터 입력

```bash
# Docker 사용
./docker/run.sh shell

# 컨테이너 내부에서
cd /app/data
/app/demo/build/bld_file      # 파일 시스템 생성
/app/demo/build/add_indx      # 인덱스 추가
/app/demo/build/add_rcrd      # 직원 추가 (대화형)
/app/demo/build/sqntl_rd      # 데이터 확인
```

### 시나리오 2: 데이터 조회

```bash
./docker/run.sh sqntl_rd
```

### 시나리오 3: 전체 워크플로우

```bash
./docker/run.sh all
```

---

## 🔗 관련 프로젝트

- **[C-ISAM Test](https://github.com/sjrhee/isam_test)**: C-ISAM 기능 테스트 및 검증을 위한 프로젝트

---

## 📝 라이선스

이 프로젝트는 교육 및 데모 목적으로 제공됩니다.

---

## 📞 문의

프로젝트 관련 문의사항이나 개선 제안이 있으시면 프로젝트 관리자에게 연락하세요.

---

**마지막 업데이트**: 2025-12-05

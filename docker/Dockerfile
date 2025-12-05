# =============================================================================
# Dockerfile for C-ISAM Demo Application
# =============================================================================
# This Dockerfile builds a containerized environment for running C-ISAM
# (Indexed Sequential Access Method) demo programs.
# Uses multi-stage build to keep the final image small.
# =============================================================================

# =============================================================================
# Stage 1: Build
# =============================================================================
FROM rockylinux:8 AS builder

# Set working directory
WORKDIR /app

# Install required packages for building
RUN dnf install -y \
    gcc \
    gcc-c++ \
    make \
    glibc-devel \
    && dnf clean all

# Copy necessary source files
# Note: Context is expected to be the project root
COPY demo/src /app/demo/src
COPY include /app/include
COPY lib /app/lib
COPY Makefile /app/Makefile

# Compile all ISAM programs
RUN make clean && make all

# =============================================================================
# Stage 2: Runtime
# =============================================================================
FROM rockylinux:8

# Set working directory
WORKDIR /app

# Install required runtime packages
RUN dnf install -y \
    shadow-utils \
    && dnf clean all

# Copy compiled executables from builder stage
COPY --from=builder /app/demo/build /app/demo/build

# Copy libraries (needed for runtime if dynamic linking is used)
COPY --from=builder /app/lib /app/lib

# Create a directory for ISAM data files
RUN mkdir -p /app/data

# Create a non-root user and set permissions
RUN useradd -m -s /bin/bash cisam && \
    chown -R cisam:cisam /app

# Switch to non-root user
USER cisam

# Set the data directory as working directory for runtime
WORKDIR /app/data

# Default command: show help
CMD ["/bin/bash", "-c", "echo '=== C-ISAM Demo Container ==='; echo ''; echo 'Available programs:'; ls -1 /app/demo/build/; echo ''; echo 'Usage: docker run -it cisam-demo /app/demo/build/<program_name>'; echo 'Example: docker run -it cisam-demo /app/demo/build/bld_file'; echo ''; echo 'For interactive shell: docker run -it cisam-demo /bin/bash'"]

# Labels
LABEL maintainer="CISAM Project"
LABEL description="C-ISAM Demo Application Container"
LABEL version="1.0"

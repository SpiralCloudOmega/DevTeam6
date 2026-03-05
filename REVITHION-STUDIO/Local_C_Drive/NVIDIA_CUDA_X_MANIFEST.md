# NVIDIA CUDA-X Library Manifest

> **Complete inventory of all NVIDIA CUDA-X libraries installed on this system.**
> Generated for the REVITHION-STUDIO development environment.

---

## Table of Contents

1. [System Information](#system-information)
2. [Conda Environments](#conda-environments)
3. [CUDA Math Libraries — System (apt)](#1-cuda-math-libraries--system-apt)
4. [CUDA Math Libraries — pip (base env, Python 3.13)](#2-cuda-math-libraries--pip-base-env-python-313)
5. [Deep Learning Libraries — pip (base env, Python 3.13)](#3-deep-learning-libraries--pip-base-env-python-313)
6. [Deep Learning Libraries — pip (cuda-x env, Python 3.12)](#4-deep-learning-libraries--pip-cuda-x-env-python-312)
7. [Quantum Computing Libraries — pip (cuda-x env, Python 3.12)](#5-quantum-computing-libraries--pip-cuda-x-env-python-312)
8. [RAPIDS Data Science Ecosystem — pip (base env, Python 3.13)](#6-rapids-data-science-ecosystem--pip-base-env-python-313)
9. [Data Processing & Partner Libraries — pip (base env, Python 3.13)](#7-data-processing--partner-libraries--pip-base-env-python-313)
10. [Image & Video Processing](#8-image--video-processing)
11. [Compression](#9-compression)
12. [CUDA Core & Bindings](#10-cuda-core--bindings)
13. [Source-Built Projects (~/projects/)](#11-source-built-projects-projects)
14. [NOT INSTALLED (Requires Special Handling)](#12-not-installed-requires-special-handling)
15. [Summary & Total Count](#summary--total-count)

---

## System Information

| Component | Details |
|-----------|---------|
| **Operating System** | WSL2 Ubuntu 24.04 LTS |
| **GPU** | NVIDIA GeForce RTX 3090 (24GB GDDR6X VRAM) |
| **CPU** | AMD Ryzen 9 5950X (16 Cores / 32 Threads) |
| **RAM** | 64GB DDR4 |
| **CUDA Toolkit** | 13.1 (V13.1.115) |
| **NVIDIA Driver** | 591.86 |
| **Python (base)** | 3.13.11 (miniconda3) |
| **Python (cuda-x)** | 3.12.12 (conda env) |

---

## Conda Environments

### Base (default)

| Property | Value |
|----------|-------|
| **Path** | `/home/autom/miniconda3` |
| **Python** | 3.13.11 |
| **Contains** | CUDA toolkit bindings, all RAPIDS, cuDNN, NCCL, CUTLASS, cuDSS, cuEquivariance, ArrayFire, Dask, CuPy, OpenCV, Triton, NVSHMEM, nvmath-python, cuStateVec |

### cuda-x

| Property | Value |
|----------|-------|
| **Path** | `/home/autom/miniconda3/envs/cuda-x` |
| **Python** | 3.12.12 |
| **Contains** | DALI, CV-CUDA, cuQuantum, cuTensor, cuTensorNet, cuDensityMat, cuPauliProp, cuStabilizer, nvJPEG, nvJPEG2K, nvTIFF, nvImgCodec, nvCOMP |
| **Activate** | `conda activate cuda-x` |

---

## 1. CUDA Math Libraries — System (apt)

These are installed as part of the CUDA 13.1 toolkit via the NVIDIA apt repository.

| # | Library | Package Name | Version | Install Method | Location |
|---|---------|-------------|---------|----------------|----------|
| 1 | cuBLAS | `libcublas-13-1` | **13.2.1.1** | apt (CUDA toolkit) | `/usr/local/cuda-13.1/` |
| 2 | cuFFT | `libcufft-13-1` | **12.1.0.78** | apt (CUDA toolkit) | `/usr/local/cuda-13.1/` |
| 3 | cuRAND | `libcurand-13-1` | **10.4.1.81** | apt (CUDA toolkit) | `/usr/local/cuda-13.1/` |
| 4 | cuSOLVER | `libcusolver-13-1` | **12.0.9.81** | apt (CUDA toolkit) | `/usr/local/cuda-13.1/` |
| 5 | cuSPARSE | `libcusparse-13-1` | **12.7.3.1** | apt (CUDA toolkit) | `/usr/local/cuda-13.1/` |
| 6 | cuFile (GDS) | `libcufile-13-1` | **1.16.1.26** | apt (CUDA toolkit) | `/usr/local/cuda-13.1/` |
| 7 | NVTX | `cuda-nvtx-13-1` | **13.1.115** | apt (CUDA toolkit) | `/usr/local/cuda-13.1/` |
| 8 | CUDA CCCL (Thrust/CUB) | `cuda-cccl-13-1` | **13.1.115** | apt (CUDA toolkit) | `/usr/local/cuda-13.1/` |

---

## 2. CUDA Math Libraries — pip (base env, Python 3.13)

These are Python wheel distributions installed via pip in the base miniconda environment.

| # | Library | pip Package | Version | Install Method | Environment |
|---|---------|------------|---------|----------------|-------------|
| 1 | cuBLAS | `nvidia-cublas-cu12` | **12.9.1.4** | pip | base (Python 3.13) |
| 2 | cuFFT | `nvidia-cufft-cu12` | **11.4.1.4** | pip | base (Python 3.13) |
| 3 | cuRAND | `nvidia-curand-cu12` | **10.3.10.19** | pip | base (Python 3.13) |
| 4 | cuSOLVER | `nvidia-cusolver-cu12` | **11.7.5.82** | pip | base (Python 3.13) |
| 5 | cuSPARSE | `nvidia-cusparse-cu12` | **12.5.10.65** | pip | base (Python 3.13) |
| 6 | cuSPARSELt | `nvidia-cusparselt-cu12` | **0.7.1** | pip | base (Python 3.13) |
| 7 | cuDSS | `nvidia-cudss-cu12` | **0.7.1.6** | pip | base (Python 3.13) |
| 8 | CUTLASS | `nvidia-cutlass` | **4.2.0.0** | pip | base (Python 3.13) |
| 9 | cuEquivariance | `cuequivariance` | **0.9.0** | pip | base (Python 3.13) |
| 10 | cuEquivariance Torch | `cuequivariance-torch` | **0.9.0** | pip | base (Python 3.13) |
| 11 | nvmath-python | `nvmath-python` | **0.8.0** | pip | base (Python 3.13) |
| 12 | cuFile | `nvidia-cufile-cu12` | **1.13.1.3** | pip | base (Python 3.13) |
| 13 | NVTX | `nvidia-nvtx-cu12` | **12.8.90** | pip | base (Python 3.13) |
| 14 | CUDA CCCL | `nvidia-cuda-cccl-cu12` | **12.9.27** | pip | base (Python 3.13) |
| 15 | nvJITLink | `nvidia-nvjitlink-cu12` | **12.8.93** | pip | base (Python 3.13) |
| 16 | cuStateVec | `custatevec-cu12` | **1.12.0** | pip | base (Python 3.13) |

---

## 3. Deep Learning Libraries — pip (base env, Python 3.13)

| # | Library | pip Package | Version | Install Method | Environment |
|---|---------|------------|---------|----------------|-------------|
| 1 | cuDNN | `nvidia-cudnn-cu12` | **9.10.2.21** | pip | base (Python 3.13) |
| 2 | NCCL | `nvidia-nccl-cu12` | **2.27.5** | pip | base (Python 3.13) |
| 3 | NVSHMEM | `nvidia-nvshmem-cu12` | **3.4.5** | pip | base (Python 3.13) |
| 4 | Triton (compiler) | `triton` | **3.6.0** | pip | base (Python 3.13) |

---

## 4. Deep Learning Libraries — pip (cuda-x env, Python 3.12)

| # | Library | pip Package | Version | Install Method | Environment |
|---|---------|------------|---------|----------------|-------------|
| 1 | NVIDIA DALI | `nvidia-dali-cuda120` | **2.0.0** | pip | cuda-x (Python 3.12) |
| 2 | CV-CUDA | `cvcuda-cu12` | **0.16.0** | pip | cuda-x (Python 3.12) |

---

## 5. Quantum Computing Libraries — pip (cuda-x env, Python 3.12)

| # | Library | pip Package | Version | Install Method | Environment |
|---|---------|------------|---------|----------------|-------------|
| 1 | cuQuantum | `cuquantum-python-cu12` | **26.1.0** | pip | cuda-x (Python 3.12) |
| 2 | cuStateVec | `custatevec-cu12` | **1.12.0** | pip | cuda-x (Python 3.12) |
| 3 | cuTensorNet | `cutensornet-cu12` | **2.11.0** | pip | cuda-x (Python 3.12) |
| 4 | cuTensor | `cutensor-cu12` | **2.5.0** | pip | cuda-x (Python 3.12) |
| 5 | cuDensityMat | `cudensitymat-cu12` | **0.4.0** | pip | cuda-x (Python 3.12) |
| 6 | cuPauliProp | `cupauliprop-cu12` | **0.2.0** | pip | cuda-x (Python 3.12) |
| 7 | cuStabilizer | `custabilizer-cu12` | **0.2.0** | pip | cuda-x (Python 3.12) |

---

## 6. RAPIDS Data Science Ecosystem — pip (base env, Python 3.13)

All RAPIDS libraries are installed from the NVIDIA pip index (`--extra-index-url https://pypi.nvidia.com`).

| # | Library | pip Package | Version | Install Method | Environment |
|---|---------|------------|---------|----------------|-------------|
| 1 | cuDF | `cudf-cu12` | **26.2.1** | pip (nvidia index) | base (Python 3.13) |
| 2 | cuML | `cuml-cu12` | **26.2.0** | pip (nvidia index) | base (Python 3.13) |
| 3 | cuGraph | `cugraph-cu12` | **26.2.0** | pip (nvidia index) | base (Python 3.13) |
| 4 | cuVS | `cuvs-cu12` | **26.2.0** | pip (nvidia index) | base (Python 3.13) |
| 5 | cuCIM | `cucim-cu12` | **26.2.0** | pip (nvidia index) | base (Python 3.13) |
| 6 | RMM | `rmm-cu12` | **26.2.0** | pip (nvidia index) | base (Python 3.13) |
| 7 | RAFT (pylibraft) | `pylibraft-cu12` | **26.2.0** | pip (nvidia index) | base (Python 3.13) |
| 8 | RAFT (raft-dask) | `raft-dask-cu12` | **26.2.0** | pip (nvidia index) | base (Python 3.13) |
| 9 | Dask CUDA | `dask-cuda` | **26.2.0** | pip (nvidia index) | base (Python 3.13) |
| 10 | Dask cuDF | `dask-cudf-cu12` | **26.2.1** | pip (nvidia index) | base (Python 3.13) |

---

## 7. Data Processing & Partner Libraries — pip (base env, Python 3.13)

| # | Library | pip Package | Version | Install Method | Environment |
|---|---------|------------|---------|----------------|-------------|
| 1 | Dask | `dask` | **2026.1.1** | pip | base (Python 3.13) |
| 2 | Distributed | `distributed` | **2026.1.1** | pip | base (Python 3.13) |
| 3 | ArrayFire | `arrayfire` | **3.8.0** | pip | base (Python 3.13) |
| 4 | CuPy | `cupy-cuda12x` | **14.0.1** | pip | base (Python 3.13) |
| 5 | Numba CUDA | `numba-cuda` | **0.22.2** | pip | base (Python 3.13) |

---

## 8. Image & Video Processing

| # | Library | pip/apt Package | Version | Install Method | Environment |
|---|---------|----------------|---------|----------------|-------------|
| 1 | OpenCV | `opencv-python` | **4.13.0** | pip | base (Python 3.13) |
| 2 | nvJPEG | `nvidia-nvjpeg-cu12` | **12.4.0.76** | pip | cuda-x (Python 3.12) |
| 3 | nvJPEG2K | `nvidia-nvjpeg2k-cu12` | **0.9.1.47** | pip | cuda-x (Python 3.12) |
| 4 | nvTIFF | `nvidia-nvtiff-cu12` | **0.6.0.78** | pip | cuda-x (Python 3.12) |
| 5 | nvImgCodec | `nvidia-nvimgcodec-cu12` | **0.7.0.11** | pip | both envs |
| 6 | PyNvVideoCodec | `PyNvVideoCodec` | **2.0.2** | pip | base (Python 3.13) |
| 7 | FFmpeg + NVENC | `ffmpeg` | **6.1.1** | apt | system |
| 8 | — H.264 NVENC encoder | `h264_nvenc` | — | built-in | system (FFmpeg) |
| 9 | — HEVC NVENC encoder | `hevc_nvenc` | — | built-in | system (FFmpeg) |
| 10 | — AV1 NVENC encoder | `av1_nvenc` | — | built-in | system (FFmpeg) |

---

## 9. Compression

| # | Library | pip Package | Version | Install Method | Environment |
|---|---------|------------|---------|----------------|-------------|
| 1 | nvCOMP | `nvidia-libnvcomp-cu12` | **5.1.0.21** | pip | both envs |

---

## 10. CUDA Core & Bindings

| # | Library | pip Package | Version | Install Method | Environment |
|---|---------|------------|---------|----------------|-------------|
| 1 | CUDA Bindings | `cuda-bindings` | **12.9.5** | pip | both envs |
| 2 | CUDA Core | `cuda-core` | **0.3.2** | pip | both envs |
| 3 | CUDA Python | `cuda-python` | **12.9.5** | pip | base (Python 3.13) |
| 4 | CUDA Toolkit | `cuda-toolkit` | **12.9.1** | pip | base (Python 3.13) |
| 5 | CUDA Runtime | `nvidia-cuda-runtime-cu12` | **12.8.90** | pip | base (Python 3.13) |
| 6 | CUDA NVRTC | `nvidia-cuda-nvrtc-cu12` | **12.9.86** | pip | base (Python 3.13) |
| 7 | CUDA NVCC | `nvidia-cuda-nvcc-cu12` | **12.9.86** | pip | base (Python 3.13) |
| 8 | CUDA CUPTI | `nvidia-cuda-cupti-cu12` | **12.8.90** | pip | base (Python 3.13) |
| 9 | NVIDIA ML | `nvidia-ml-py` | **13.590.48** | pip | base (Python 3.13) |

---

## 11. Source-Built Projects (~/projects/)

| # | Library | Directory | Build Status | Description |
|---|---------|-----------|-------------|-------------|
| 1 | AmgX | `~/projects/AMGX` | ✅ Built (`libamgx.a` 454MB, `libamgxsh.so` 276MB) | GPU-accelerated algebraic multigrid solver |
| 2 | Gunrock | `~/projects/gunrock` | ✅ Built (15 examples compiled) | GPU graph analytics library |
| 3 | MAGMA | `~/projects/magma` | ⚠️ Source only (build pending) | Dense linear algebra on GPU |
| 4 | Morpheus | `~/projects/Morpheus` | ⚠️ Cloned (Docker-based build) | Cybersecurity AI framework |
| 5 | NeMo Curator | `~/projects/NeMo-Curator` | ⚠️ Cloned (pip install failed) | Data curation for LLM training |
| 6 | NeMo Agent Toolkit | `~/projects/NeMo-Agent-Toolkit` | ⚠️ Cloned | AI agent framework |
| 7 | NVIDIA Optical Flow SDK | `~/projects/NVIDIAOpticalFlowSDK` | ⚠️ Cloned | Pixel-level motion estimation |
| 8 | VideoProcessingFramework | `~/projects/VideoProcessingFramework` | ⚠️ Cloned | Python video codec bindings |

---

## 12. NOT INSTALLED (Requires Special Handling)

| # | Library | Reason Not Installed | Workaround / Notes |
|---|---------|---------------------|---------------------|
| 1 | TensorRT | Python 3.12 build fails (`wheel_stub.buildapi`) | Needs Python ≤3.11 environment |
| 2 | TensorRT-LLM | Depends on TensorRT | Needs Python ≤3.11 environment |
| 3 | cuPQC (Post-Quantum Crypto) | Not available on PyPI | May be enterprise/preview only |
| 4 | RAPIDS Accelerator for Spark | Requires Apache Spark runtime | Install when Spark is needed |

---

## Summary & Total Count

| Category | Count |
|----------|-------|
| pip packages (across both envs) | **57** |
| System (apt) packages from CUDA toolkit | **36** |
| Source-built projects in `~/projects/` | **8** |
| Not installed (special handling) | **3** |
| **Grand Total** | **~100+ NVIDIA CUDA-X components** |

### Breakdown by Environment

| Environment | Library Count (pip) | Key Domains |
|-------------|-------------------|-------------|
| **base** (Python 3.13) | ~45 packages | RAPIDS, cuDNN, NCCL, CUTLASS, cuDSS, cuEquivariance, CuPy, Triton, Dask, OpenCV |
| **cuda-x** (Python 3.12) | ~12 packages | DALI, CV-CUDA, cuQuantum, cuTensor, cuTensorNet, cuDensityMat, nvJPEG, nvJPEG2K, nvTIFF |
| **System (apt)** | 36 packages | CUDA 13.1 toolkit (cuBLAS, cuFFT, cuRAND, cuSOLVER, cuSPARSE, cuFile, NVTX, CCCL) |

### Quick Reference — Key Commands

```bash
# Activate cuda-x environment
conda activate cuda-x

# Check CUDA version
nvcc --version

# Check driver version
nvidia-smi

# List all NVIDIA pip packages (base)
pip list | grep -i nvidia

# List all NVIDIA pip packages (cuda-x)
conda run -n cuda-x pip list | grep -i nvidia

# Check RAPIDS versions
python -c "import cudf; print(cudf.__version__)"
python -c "import cuml; print(cuml.__version__)"

# Check cuDNN version
python -c "import torch; print(torch.backends.cudnn.version())"
```

---

*Installation session: `06ef625c` — All libraries installed/verified during this session.*
*Manifest path: `REVITHION-STUDIO/Local_C_Drive/NVIDIA_CUDA_X_MANIFEST.md`*

---

## Addendum — Additional Conda Environments (added session 06ef625c cont.)

### tensorrt (Python 3.11.14)
- Path: `/home/autom/miniconda3/envs/tensorrt`
- **TensorRT**: 10.15.1.29 ✅
- Activate: `conda activate tensorrt`

### tensorrt-py310 (Python 3.10.19)
- Path: `/home/autom/miniconda3/envs/tensorrt-py310`
- **TensorRT**: 10.15.1.29 ✅
- **TensorRT-LLM**: 0.13.0 (download pending — 1.9GB wheel, retry when network is clear)
- Activate: `conda activate tensorrt-py310`

### System Python
- `/usr/bin/python` → python3 (via `python-is-python3` package)
- `/usr/bin/python3` → Python 3.12.3 (system)
- Windows: Python 3.14 at `C:\Users\autom\AppData\Local\Programs\Python\`

### VS Code Configuration
- `python.defaultInterpreterPath`: `/home/autom/miniconda3/bin/python`
- `python.condaPath`: `/home/autom/miniconda3/bin/conda`
- Configured at: workspace, WSL machine, and global levels

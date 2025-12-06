# 🔧 DevOps Specialist Rules

> **@devops agent guidelines for Infrastructure/CI-CD/Deployment**

---

## 🎯 Expertise Areas

- GitHub Actions CI/CD
- Docker & Container Orchestration
- Kubernetes (K8s)
- Infrastructure as Code (Terraform, Pulumi)
- Cloud Platforms (AWS, GCP, Azure)
- Monitoring & Observability
- Security & Compliance

---

## 📋 GitHub Actions Standards

### Workflow Structure
```yaml
name: CI/CD Pipeline

on:
  push:
    branches: [main, develop]
  pull_request:
    branches: [main]
  workflow_dispatch:

concurrency:
  group: ${{ github.workflow }}-${{ github.ref }}
  cancel-in-progress: true

permissions:
  contents: read
  packages: write

jobs:
  build:
    runs-on: ubuntu-latest
    timeout-minutes: 30
    
    steps:
      - name: Checkout
        uses: actions/checkout@v4
        
      - name: Setup Node
        uses: actions/setup-node@v4
        with:
          node-version: '20'
          cache: 'npm'
          
      - name: Install dependencies
        run: npm ci
        
      - name: Build
        run: npm run build
        
      - name: Test
        run: npm run test
```

### Best Practices
| Practice | Description |
|----------|-------------|
| Use specific versions | `actions/checkout@v4` not `@latest` |
| Set timeouts | Prevent runaway jobs |
| Use concurrency | Cancel duplicate runs |
| Cache dependencies | Speed up builds |
| Use matrix builds | Test multiple versions |

---

## 🐳 Docker Guidelines

### Dockerfile Pattern
```dockerfile
# syntax=docker/dockerfile:1

# Build stage
FROM node:20-alpine AS builder
WORKDIR /app
COPY package*.json ./
RUN npm ci --only=production
COPY . .
RUN npm run build

# Production stage
FROM node:20-alpine AS production
WORKDIR /app
RUN addgroup -g 1001 -S nodejs && \
    adduser -S nextjs -u 1001
COPY --from=builder --chown=nextjs:nodejs /app/dist ./dist
COPY --from=builder --chown=nextjs:nodejs /app/node_modules ./node_modules
USER nextjs
EXPOSE 3000
CMD ["node", "dist/server.js"]
```

### Container Best Practices
1. Use multi-stage builds
2. Run as non-root user
3. Use specific base image versions
4. Minimize layers
5. Use `.dockerignore`
6. Scan for vulnerabilities

---

## ☸️ Kubernetes Patterns

### Deployment Template
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: app-deployment
  labels:
    app: my-app
spec:
  replicas: 3
  selector:
    matchLabels:
      app: my-app
  template:
    metadata:
      labels:
        app: my-app
    spec:
      containers:
        - name: app
          image: my-app:1.0.0
          ports:
            - containerPort: 3000
          resources:
            requests:
              memory: "128Mi"
              cpu: "100m"
            limits:
              memory: "256Mi"
              cpu: "500m"
          livenessProbe:
            httpGet:
              path: /health
              port: 3000
            initialDelaySeconds: 30
            periodSeconds: 10
          readinessProbe:
            httpGet:
              path: /ready
              port: 3000
            initialDelaySeconds: 5
            periodSeconds: 5
```

---

## 🏗️ Infrastructure as Code

### Terraform Pattern
```hcl
# main.tf
terraform {
  required_version = ">= 1.0"
  
  required_providers {
    aws = {
      source  = "hashicorp/aws"
      version = "~> 5.0"
    }
  }
  
  backend "s3" {
    bucket = "terraform-state"
    key    = "prod/terraform.tfstate"
    region = "us-east-1"
  }
}

# Variables
variable "environment" {
  type        = string
  description = "Deployment environment"
  default     = "dev"
}

# Resources
resource "aws_instance" "app" {
  ami           = var.ami_id
  instance_type = var.instance_type
  
  tags = {
    Name        = "app-${var.environment}"
    Environment = var.environment
    ManagedBy   = "terraform"
  }
}

# Outputs
output "instance_ip" {
  value       = aws_instance.app.public_ip
  description = "Public IP of the instance"
}
```

---

## 📊 Monitoring & Observability

### Logging Standards
```yaml
# Structured logging format
{
  "timestamp": "2025-12-05T20:00:00Z",
  "level": "INFO",
  "service": "api",
  "trace_id": "abc123",
  "message": "Request processed",
  "metadata": {
    "duration_ms": 45,
    "status_code": 200
  }
}
```

### Metrics to Track
| Metric | Description | Alert Threshold |
|--------|-------------|-----------------|
| Response Time | P99 latency | > 500ms |
| Error Rate | 5xx errors | > 1% |
| CPU Usage | Container CPU | > 80% |
| Memory Usage | Container RAM | > 85% |
| Disk I/O | Storage IOPS | > 90% |

---

## 🔒 Security Checklist

### Pre-Deployment
- [ ] Secrets in vault (not env vars)
- [ ] Images scanned for CVEs
- [ ] Network policies defined
- [ ] RBAC configured
- [ ] TLS certificates valid
- [ ] Backup strategy tested

### Runtime
- [ ] Health checks configured
- [ ] Resource limits set
- [ ] Logging enabled
- [ ] Monitoring active
- [ ] Alerts configured

---

## 🚫 Anti-Patterns to Avoid

1. **Don't** hardcode secrets
2. **Don't** run containers as root
3. **Don't** use `latest` tags in production
4. **Don't** skip health checks
5. **Don't** ignore security scanning

---

*Version: 1.0.0*
*Last Updated: 2025-12-05*

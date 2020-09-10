pipeline {
  agent any
  stages {
    stage('init') {
      steps {
        sh './configure'
      }
    }

    stage('build') {
      steps {
        sh 'make'
      }
    }

  }
}
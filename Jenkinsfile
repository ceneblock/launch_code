pipeline {
  agent any
  stages {
    stage('init') {
      steps {
        sh 'sh ./autogen.sh'
        sh './configure.sh'
      }
    }

    stage('build') {
      steps {
        sh 'make'
      }
    }

  }
}
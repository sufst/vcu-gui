pipeline {
  agent any
  
  stages {
    stage ('Prepare') {
      steps {
        echo '==== Running prepare ===='
        sh 'git submodule init'
        sh 'git submodule update'
      }
    }

    stage ('Build') {
      steps {
        echo '==== Running build ===='
        sh 'mkdir build'
        sh 'cd build && cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON'
        sh 'cd build && make -j4'
      }
    }

    stage ('Lint') {
      steps {
        echo '==== Linting ===='
        //sh 'ln -s build/compile_commands.json ./compile_commands.json'
        sh 'trunk --ci check'
      }
    }
  }
}
#!/usr/bin/env groovy

node('linux_xenial_unprovisioned') {

  stage('checkout') {
    checkout scm
  }

  stage('setup') {
    sh 'sudo bash setup/ci/linux_ubuntu_xenial.bash'
  }

  stage('build and test') {
    sh 'bash setup/ci/jenkins.bash'
  }

  cleanWs(notFailBuild: true)

}

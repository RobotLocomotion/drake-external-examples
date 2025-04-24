#!/usr/bin/env groovy

/* SPDX-License-Identifier: MIT-0 */

// This file defines the Jenkins job pipeline for the main and PR builds
// on this repository. It's also used as a template for the pipeline for
// Drake's linux-jammy-unprovisioned-external-examples job:
// https://drake-jenkins.csail.mit.edu/job/linux-jammy-unprovisioned-external-examples/.
// Notably, the job uses some additional parameters to the setup and
// ci_build_test scripts not used here.
// When updating this file, please also update the pipeline definition
// in drake-jenkins-jobs accordingly.

if (env.BRANCH_NAME == 'main') {
  def triggers = []
  triggers << cron('H H(7-8) * * *')
  properties ([
    pipelineTriggers(triggers)
  ])
}

def examples = ['drake_bazel_external', 'drake_bazel_external_legacy', 'drake_cmake_external']
def jobs = [:]

for (example in examples) {
  def name = example
  jobs[name] = {
    node('linux-jammy-unprovisioned') {
      timeout(600) {
        ansiColor('xterm') {
          try {
            dir('src') {
              stage('checkout') {
                checkout scm
              }
            }
            dir('src/' + name) {
              stage(name + ' setup') {
                sh '.github/setup'
              }
              stage(name + ' build and test') {
                sh '.github/ci_build_test'
              }
            }
          } catch (e) {
            if (env.BRANCH_NAME == 'main') {
              emailext (
              subject: "Build failed in Jenkins: ${env.JOB_NAME} #${env.BUILD_NUMBER}",
              body: "See <${env.BUILD_URL}display/redirect?page=changes>",
              recipientProviders: [[$class: 'DevelopersRecipientProvider']],
              to: '$DEFAULT_RECIPIENTS',
              )
            }
            throw e
          }
        }
      }
    }
  }
}

parallel jobs

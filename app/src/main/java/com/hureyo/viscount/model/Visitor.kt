package com.hureyo.viscount.model

import com.google.firebase.database.Exclude
import com.google.firebase.database.IgnoreExtraProperties

@IgnoreExtraProperties
data class Visitor(
    val masuk: Int? = null,
    val keluar: Int? = null,
    val total: Int? = null
)
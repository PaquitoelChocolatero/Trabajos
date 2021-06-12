package es.uc3m.g1.musey.model.api.lastfm

import com.google.gson.JsonDeserializationContext
import com.google.gson.JsonDeserializer
import com.google.gson.JsonElement
import com.google.gson.annotations.JsonAdapter
import com.google.gson.annotations.SerializedName
import java.lang.reflect.Type

data class Album(
        val artist: String,
        val title: String,
        @JsonAdapter(CoverListDeserializer::class)
        @SerializedName("image") var covers: Map<String, String> = emptyMap(),
) {
    class CoverListDeserializer : JsonDeserializer<Map<String, String>> {
        override fun deserialize(
                json: JsonElement?,
                typeOfT: Type?,
                context: JsonDeserializationContext?
        ): Map<String, String> {
            val map: MutableMap<String, String> = mutableMapOf()
            json?.asJsonArray?.forEach { cover ->
                cover.asJsonObject?.run {
                    val url = get("#text").asString ?: return emptyMap()
                    val size = get("size").asString ?: return emptyMap()
                    map[size] = url
                }
            }
            return map
        }
    }
}
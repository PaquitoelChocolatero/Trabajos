package es.uc3m.g1.musey.viewModel

import android.app.Application
import android.content.res.Resources
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.viewModelScope
import es.uc3m.g1.musey.R
import es.uc3m.g1.musey.model.Repository
import es.uc3m.g1.musey.model.api.lastfm.Track
import es.uc3m.g1.musey.model.firebase.FirebaseInstance
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.security.InvalidKeyException

class MainViewModel(val app: Application) : AndroidViewModel(app) {

    val repository: Repository = Repository(app.applicationContext)
    val firebase: FirebaseInstance = repository.firebase

    val error: MutableLiveData<String> = MutableLiveData()

    val listSearch: MutableLiveData<List<Track>> = MutableLiveData()
    val listRecommend: MutableLiveData<List<Track>> = MutableLiveData()

    private var tracksSearched: List<Track> = emptyList()
        set(value) {
            field = value
            listRecommend.postValue(value)
            tracksSearched.forEach {
                viewModelScope.launch(Dispatchers.IO) {
                    it.album = repository.getInfo(it)?.album
                    listSearch.postValue(tracksSearched)
                }
            }
        }

    private var tracksRecommend: List<Track> = emptyList()
        set(value) {
            field = value
            listRecommend.postValue(value)
            tracksRecommend.forEach {
                viewModelScope.launch(Dispatchers.IO) {
                    it.album = repository.getInfo(it)?.album
                    listRecommend.postValue(tracksRecommend)
                }
            }
        }

    var track: Track? = null
        set(value) {
            field = value
            if (value != null) {
                viewModelScope.launch(Dispatchers.IO) {
                    callApi {
                        tracksRecommend = repository.getSimilar(value)
                    }
                }
            } else {
                tracksRecommend = emptyList()
            }
        }

    var search: String? = null
        set(value) {
            field = value
            if (value != null) {
                viewModelScope.launch(Dispatchers.IO) {
                    callApi {
                        tracksSearched = repository.getTrack(value)
                    }
                }
            }
        }

    private suspend fun callApi(block: suspend () -> Unit) {
        try {
            block()
        } catch (e: javax.net.ssl.SSLHandshakeException) {
            error.postValue(
                app.applicationContext.getString(R.string.ssl_pin_error)
            )
        } catch (e: Resources.NotFoundException) {
            error.postValue(
                app.applicationContext.getString(R.string.api_key_error)
            )
        } catch (e: InvalidKeyException) {
            error.postValue(
                app.applicationContext.getString(R.string.bad_api_key)
            )
        }
    }
}